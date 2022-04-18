#include "SaveSystem/Runnable_Save.h"
#include "Misc/DateTime.h"
#include "SaveSystem/Task_SaveGameToSlot.h"
#include "FL_Logger.h"

//***********************************************************
//Thread Worker Starts as NULL, prior to being instanced
//		This line is essential! Compiler error without it
Runnable_Save* Runnable_Save::Instance = nullptr;
//***********************************************************

Runnable_Save::Runnable_Save(const TMap<FString, USaveGame*> SaveGames, I_AsyncSavingCallback* Listener) {
	this->Listener = Listener;
	this->SaveGames = SaveGames;

	//create thread pool
	Pool = FQueuedThreadPool::Allocate();
	const int32 numThreadsInPool = FPlatformMisc::NumberOfWorkerThreadsToSpawn();
	verify(Pool->Create(numThreadsInPool, 128 * 1024));

	//init task counter
	RunningTaskCounter.Set(SaveGames.Num());
	//UE_LOG(LogGame, Verbose, TEXT("SaveGame Objects to save to disk: %i"), SaveGames.Num());
	Thread = FRunnableThread::Create(this, TEXT("SaveAll"), 0, TPri_BelowNormal);
}

Runnable_Save::~Runnable_Save() {
	delete Thread;
	Thread = nullptr;

	if (Pool) {
		Pool->Destroy(); // seems to force the game to wait until saving pool is completed
		delete Pool;
		Pool = nullptr;
	}
}

Runnable_Save* Runnable_Save::SaveAll(TMap<FString, USaveGame*> _SaveGames, I_AsyncSavingCallback* _Listener) {
	// todo: should react to SupportsMultiThreading() == false
	// create new instance if none exists
	if (!Instance && FPlatformProcess::SupportsMultithreading()) {
		Instance = new Runnable_Save(_SaveGames, _Listener);
	}

	return Instance;
}

void Runnable_Save::Shutdown() {
	if (Instance) {
		Instance->EnsureCompletion();
		delete Instance;
		Instance = nullptr;
	}
}

bool Runnable_Save::IsThreadFinished() {
	if (Instance) {
		return Instance->IsFinished();
	}
	else {
		return true;
	}
}

uint32 Runnable_Save::Run() {
	const FDateTime Start = FDateTime::Now();

	for (TPair<FString, USaveGame*> saveGame : SaveGames) {
		// save to disk

		(new FAutoDeleteAsyncTask<Task_SaveGameToSlot>(saveGame.Value, saveGame.Key, &RunningTaskCounter))->
			StartBackgroundTask(Pool);

		// stop this task
		if (StopTaskCounter.GetValue() != 0) {
			
			UFL_Logger::OutputLog(Verbose, FString::Printf(TEXT("Execution stopped, took: %fs"), (FDateTime::Now() - Start).GetTotalSeconds()));
			return 0;
		}
	}

	while (!IsFinished()) { FPlatformProcess::Sleep(0.1); }

	UFL_Logger::OutputLog(Verbose, FString::Printf(TEXT("Execution complete, took: %fs"), (FDateTime::Now() - Start).GetTotalSeconds()));

	return 0;
}

void Runnable_Save::Stop() {
	StopTaskCounter.Increment();
}

bool Runnable_Save::IsFinished() const {
	return RunningTaskCounter.GetValue() == 0 || StopTaskCounter.GetValue() != 0;
}

void Runnable_Save::Exit() {
	UFL_Logger::OutputLog(Verbose, TEXT("Runnable Save exit"));

	// callback
	AsyncTask(ENamedThreads::GameThread, [this]() { Listener->OnSaveFinished(); });
}

void Runnable_Save::EnsureCompletion() {
	Stop();
	Thread->WaitForCompletion();
}
