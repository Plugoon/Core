#include "PC_Core.h"

#include "AC_Core.h"
#include "CM_Core.h"
#include "FL_Logger.h"
#include "SaveSystem/FL_SaveSystem.h"
#include "GM_Core.h"
#include "O_Exception.h"
#include "GameFramework/PlayerController.h"

APC_Core::APC_Core() {
	PrimaryActorTick.bCanEverTick = true;
	CheatManager = NewObject<UCM_Core>(this, TEXT("Cheatmanager"));
}

void APC_Core::LogMessage(const FString& Message, const ELogLevel Level) {
	if (DisplayName == "") {
		DisplayName = GetName();
	}

	if (Level >= LogLevel) {
		const FString LogMessage = FString::Printf(TEXT("%s: %s"), *DisplayName, *Message);
		UFL_Logger::OutputLog(Level, LogMessage);
		if (bDebug) {
			GEngine->AddOnScreenDebugMessage(-1, PrintTime, UFL_Logger::GetLogLevelColor(Level), LogMessage);
		}
	}
}

void APC_Core::BeginPlay() {
	Init();

	Super::BeginPlay();
}

void APC_Core::Init() {
	LogMessage("Init...", ELogLevel::Log);

	if (Savable) {
		LogMessage("IsSavable->getSaveGame", ELogLevel::Verbose);
		try {
			SaveGame = SaveGame = UFL_SaveSystem::GetSaveSystem()->GetSaveGame(this, Type);
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

	LogMessage("Call OnPlayerController Loaded", ELogLevel::Verbose);
	Execute_OnLoadFinished(this);
	OnPlayerControllerLoaded.Broadcast();
}

void APC_Core::UpdateSaveGame() {
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

void APC_Core::UpdateSaveGameImplementation_Implementation() {
	LogMessage(TEXT("UpdateSaveGameImplementation not overwritten in Blueprint!"), ELogLevel::Warning);
}
