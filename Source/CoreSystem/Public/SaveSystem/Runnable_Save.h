#pragma once

#include "CoreMinimal.h"
#include "SaveSystem/I_AsyncSavingCallback.h"
#include "GameFramework/SaveGame.h"

class CORESYSTEM_API Runnable_Save : public FRunnable {
public:
	static Runnable_Save* Instance;

private:
	FRunnableThread* Thread;
	FThreadSafeCounter StopTaskCounter;
	FThreadSafeCounter RunningTaskCounter;

	bool bIsFinished = false;
	TMap<FString, USaveGame*> SaveGames;

	FQueuedThreadPool* Pool;
	I_AsyncSavingCallback* Listener;
	
public:
	~Runnable_Save();

	/**
	* \brief Start the thread and the worker.
	* \brief This code ensures only one saving thread will be able to run at a time.
	* \return handle to the newly started instance
	*/
	static Runnable_Save* SaveAll(TMap<FString, USaveGame*> _SaveGames, I_AsyncSavingCallback* _Listener);

	/**
	* \brief Shuts down the thread
	*/
	static void Shutdown();

	/**
	* \brief Check if the thread has finished execution.
	*/
	static bool IsThreadFinished();

private:
	Runnable_Save(const TMap<FString, USaveGame*> SaveGames, I_AsyncSavingCallback* Listener);
	bool IsFinished() const;

	uint32 Run() override;
	void Stop() override;
	bool Init() override {return true;};
	void Exit() override;

	// make sure the thread has finished execution
	void EnsureCompletion();
};
