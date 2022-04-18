#include "A_Core.h"

#include "AC_Core.h"
#include "FL_Logger.h"
#include "O_Exception.h"
#include "SaveSystem/FL_SaveSystem.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AA_Core::AA_Core() {
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AA_Core::BeginPlay() {
	Super::BeginPlay();
	Init();
}

// Called every frame
void AA_Core::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AA_Core::LogMessage(const FString& Message, ELogLevel Level) {
	if (DisplayName == "") {
		DisplayName = UKismetSystemLibrary::GetDisplayName((UObject*)this);
	}

	if (Level >= LogLevel) {
		const FString LogMessage = FString::Printf(TEXT("%s: %s"), *DisplayName, *Message);
		UFL_Logger::OutputLog(Level, LogMessage);
		if (bDebug) {
			GEngine->AddOnScreenDebugMessage(-1, PrintTime, UFL_Logger::GetLogLevelColor(Level), LogMessage);
		}
	}
}

void AA_Core::Init() {
	LogMessage("Init...", ELogLevel::Log);

	if (Savable) {
		LogMessage("IsSavable->getSaveGame", ELogLevel::Verbose);
		try {
			SaveGame = UFL_SaveSystem::GetSaveSystem()->GetSaveGame(this, Type);
		} catch (UO_Exception *ex) {
			LogMessage(ex->Message, Warning);
		}
	}

	//Initialize all Components
	LogMessage("Initialize Components...", ELogLevel::Verbose);
	TArray<UAC_Core*> Components;
	GetComponents<UAC_Core>(Components);
	for (UAC_Core* Comp : Components) {
		Comp->Init();
	}

	LogMessage("Call OnActorLoaded", ELogLevel::Verbose);
	Execute_OnLoadFinished(this);
	OnActorLoaded.Broadcast();
}

void AA_Core::UpdateSaveGame() {
	if (!SaveGame) {
		LogMessage(TEXT("The SaveGame was not loaded before calling UpdateSaveGame! Update was ignored."), Warning);
		return;
	}
	
	Execute_UpdateSaveGameImplementation(this);
	try {
		UFL_SaveSystem::GetSaveSystem()->UpdateSaveGame(this, SaveGame);
	} catch (UO_Exception *ex) {
		LogMessage(ex->Message, Warning);
	}
}

void AA_Core::UpdateSaveGameImplementation_Implementation() {
	LogMessage(TEXT("UpdateSaveGameImplementation not overwritten in Blueprint!"), ELogLevel::Error);
}
