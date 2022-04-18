#include "C_Core.h"

#include "AC_Core.h"
#include "FL_Logger.h"
#include "SaveSystem/FL_SaveSystem.h"
#include "GI_Core.h"
#include "O_Exception.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AC_Core::AC_Core() {
	PrimaryActorTick.bCanEverTick = true;
}

void AC_Core::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AC_Core::LogMessage(const FString& Message, const ELogLevel Level) {
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

AC_Core* AC_Core::ReplacePlayerCharacter(TSubclassOf<AC_Core> _Type, const FTransform& _Transform, AActor* _Owner,
                                         APawn* _Instigator, ESpawnActorCollisionHandlingMethod _CollisionHandling) {
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = _Instigator;
	SpawnParameters.Owner = _Owner;
	SpawnParameters.SpawnCollisionHandlingOverride = _CollisionHandling;
	SpawnParameters.bDeferConstruction = true; // wait with construction for possession

	AC_Core* NewPlayer = (AC_Core*)GWorld->SpawnActor(_Type, &_Transform, SpawnParameters);

	if (!NewPlayer) {
		UFL_Logger::OutputLog(Warning, TEXT("No Character could be spawned!"));
	}

	// possess new player character and delete old one
	ACharacter* OldPlayer = UGameplayStatics::GetPlayerCharacter(NewPlayer, 0);
	UGameplayStatics::GetPlayerController(NewPlayer, 0)->Possess(NewPlayer);
	OldPlayer->Destroy();

	// run Construction and BeginPlay
	NewPlayer->FinishSpawning(_Transform);

	return NewPlayer;
}

void AC_Core::BeginPlay() {
	Init();

	Super::BeginPlay();
}

void AC_Core::Init() {
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

	LogMessage("Call OnPlayerCharacter Loaded", ELogLevel::Verbose);
	Execute_OnLoadFinished(this);
	OnPlayerCharacterLoaded.Broadcast();
}

void AC_Core::UpdateSaveGame() {
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

void AC_Core::UpdateSaveGameImplementation_Implementation() {
	LogMessage(TEXT("UpdateSaveGameImplementation not overwritten in Blueprint!"), ELogLevel::Warning);
}
