#include "SaveSystem/AC_SaveSystem.h"

#include "EX_InvalidArgument.h"
#include "EX_ProcessAlreadyRunning.h"
#include "FL_Exception.h"
#include "FL_Logger.h"
#include "SaveSystem/FL_SaveSystem.h"
#include "AC_Core.h"
#include "A_Core.h"
#include "C_Core.h"
#include "GM_Core.h"
#include "PC_Core.h"
#include "GI_Core.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAC_SaveSystem::UAC_SaveSystem() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_SaveSystem::BeginPlay() {
	Super::BeginPlay();
}


void UAC_SaveSystem::SaveAll() {
	//Check if is saving
	if (IsCurrentlySaving()) {
		throw UFL_Exception::CreateException(UEX_ProcessAlreadyRunning::StaticClass(), "Save System is already saving!", this);
	}

	//Check if savable content is available (skip if not)
	if (ChangedSaveGames.Num() == 0) {
		LogMessage("Nothing saved as the number of changed SaveGames is 0.", ELogLevel::Log);
		OnSaveFinished();
		return;
	}

	//Copy changed SaveGame map to temporary map
	SavingSaveGames = ChangedSaveGames;
	LogMessage(FString::Printf(TEXT("Saving %i SaveGames."), SavingSaveGames.Num()), ELogLevel::Verbose);

	// append changed SaveGames map to all SaveGames map (overwrite duplicate keys)
	AllSaveGames.Append(ChangedSaveGames);
	ChangedSaveGames.Empty();

	LogMessage(TEXT("Saving Thread started"), ELogLevel::Log);

	// delete finished runnable (if necessary)
	Runnable_Save::Shutdown();
	// start multi threaded saving
	RunnableTask = Runnable_Save::SaveAll(SavingSaveGames, this);
}


void UAC_SaveSystem::SaveAll(FS_Exception &Exception) {
	try {
		SaveAll();
	} catch (UO_Exception* e){
		Exception = e->ConvertToStruct();
	}
}

void UAC_SaveSystem::EndPlay(EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	RunnableTask->Shutdown();
}

void UAC_SaveSystem::LogMessage(const FString& Message, ELogLevel Level) {
	if (DisplayName == "") {
		DisplayName = GetName();
	}

	if (Level >= LogLevel) {
		const FString Owner = UKismetSystemLibrary::GetDisplayName((UObject*)GetOwner());
		const FString LogMessage = FString::Printf(TEXT("%s->%s: %s"), *Owner, *DisplayName, *Message);
		UFL_Logger::OutputLog(Level, LogMessage);
		if (bDebug) {
			GEngine->AddOnScreenDebugMessage(-1, PrintTime, UFL_Logger::GetLogLevelColor(Level), LogMessage);
		}
	}
}

USaveGame* UAC_SaveSystem::GetSaveGame(UObject* Source, TSubclassOf<USaveGame> Type) {
	LogMessage("Function getSaveGame(): started...", ELogLevel::VeryVerbose);
	try {
		const FString path = getSaveGamePath(Source);
		USaveGame* SG = getSaveGameFromPath(path, Type);
		LogMessage(FString::Printf(TEXT("Function getSaveGame(): finished successfully for: %s"), *path), ELogLevel::Verbose);

		AllSaveGames.Add(path, SG);

		return SG;

	}
	catch (UO_Exception* Exception) {
		LogMessage(Exception->Message, ELogLevel::Error);
		return nullptr;
	}
}

bool UAC_SaveSystem::IsCurrentlySaving() {
	return !Runnable_Save::IsThreadFinished();
}

void UAC_SaveSystem::UpdateSaveGame(UObject* Source, USaveGame* SaveGame) {
	const FString path = getSaveGamePath(Source);

	LogMessage(FString::Printf(TEXT("Updating SaveGame for: %s"), *path), ELogLevel::Verbose);
	ChangedSaveGames.Add(path, SaveGame);
}

void UAC_SaveSystem::Subscribe(TScriptInterface<II_SaveSystemListener> Listener) {
	Listeners.AddUnique(Listener);
}

void UAC_SaveSystem::Unsubscribe(TScriptInterface<II_SaveSystemListener> Listener) {
	Listeners.Remove(Listener);
}

void UAC_SaveSystem::OnSaveFinished() {
	// delete TMP SaveGame array
	SavingSaveGames.Empty();

	for (TScriptInterface<II_SaveSystemListener> Listener : Listeners) {
		Listener->Execute_OnSaveFinished(Listener.GetObject());
	}
	
	LogMessage("Saving finished.", ELogLevel::Log);
}

FString UAC_SaveSystem::getSaveGamePath(UObject* source) {
	FString Result = "";
	LogMessage("Function getSaveGamePath(): started...", ELogLevel::Verbose);

	if (!UFL_SaveSystem::IsCoreClass(source)) {
		LogMessage("Function getSaveGamePath(): source is no CoreClass -> throw Exception", ELogLevel::Warning);
		throw UFL_Exception::CreateException(UEX_InvalidArgument::StaticClass(), "source is not a base Class", this);
	}

	const AA_Core* Actor = Cast<AA_Core>(source);
	if (Actor) {
		const FString LevelName = GetWorld()->GetName();
		const FString ActorName = UKismetSystemLibrary::GetDisplayName(Actor);
		Result = FString::Printf(TEXT("World/%s/%s"), *LevelName, *ActorName);
	}

	const AGM_Core* GameMode = Cast<AGM_Core>(source);
	if (GameMode) {
		const FString aName = UKismetSystemLibrary::GetDisplayName(GameMode);
		Result = FString::Printf(TEXT("Game/%s"), *aName);
	}

	const APC_Core* PlayerController = Cast<APC_Core>(source);
	if (PlayerController) {
		const FString aName = UKismetSystemLibrary::GetDisplayName(PlayerController);
		//Result = FString::Printf(TEXT("Player/%s"), *aName);
		Result = TEXT("Player/PC_Core");
	}

	const AC_Core* Character = Cast<AC_Core>(source);
	if (Character) {
		//check if Player Character
		if (Character == GetWorld()->GetFirstPlayerController()->GetPawn()) {
			const FString aName = UKismetSystemLibrary::GetDisplayName(Character);
			//Result = FString::Printf(TEXT("Player/%s"), *aName);
			Result = TEXT("Player/C_Core");
		}
		else {
			const FString LevelName = GetWorld()->GetName();
			const FString ActorName = UKismetSystemLibrary::GetDisplayName(Character);
			Result = FString::Printf(TEXT("World/%s/%s"), *LevelName, *ActorName);
		}
	}

	const UAC_Core* ActorComponent = Cast<UAC_Core>(source);
	if (ActorComponent) {
		if (!UFL_SaveSystem::IsCoreClass(ActorComponent->GetOwner())) {
			LogMessage("Function getSaveGamePath(): source owner is no CoreClass -> throw Exception",
					   ELogLevel::Verbose);
			throw UFL_Exception::CreateException(UEX_InvalidArgument::StaticClass(),
										  "owner of source is not a base Class --> will never save!", this);
		}
		//if PlayerController or PlayerCharacter
		if (
			ActorComponent->GetOwner() == GetWorld()->GetFirstPlayerController()
			|| ActorComponent->GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn()
		) {
			const FString ActorName = UKismetSystemLibrary::GetDisplayName(ActorComponent->GetOwner());
			Result = FString::Printf(TEXT("Player/Player-%s"), *ActorComponent->GetName());
		}
			//if GameMode
		else if (GetWorld()->GetAuthGameMode() == ActorComponent->GetOwner()) {
			const FString ActorName = UKismetSystemLibrary::GetDisplayName(ActorComponent->GetOwner());
			Result = FString::Printf(TEXT("Game/%s-%s"), *ActorName, *ActorComponent->GetName());
		}
			//if Actor
		else {
			const FString LevelName = GetWorld()->GetName();
			const FString ActorName = UKismetSystemLibrary::GetDisplayName(ActorComponent->GetOwner());
			const FString ComponentName = ActorComponent->GetName();
			Result = FString::Printf(TEXT("World/%s/%s-%s"), *LevelName, *ActorName, *ComponentName);
		}
	}
	if (Result == "") {
		LogMessage("Function getSaveGamePath(): source is unknown base class -> throw Exception", ELogLevel::Verbose);
		throw UFL_Exception::CreateException(UEX_InvalidArgument::StaticClass(), "source is unknown base class", this);
	}
	LogMessage(FString::Printf(TEXT("Function getSaveGamePath(): return->%s"), *Result), ELogLevel::Verbose);

	// add Key
	UGI_Core* instance = (UGI_Core*) UGameplayStatics::GetGameInstance(this);
	if (instance) {
		FString Key = instance->SaveGameKey;
		Result = FString::Printf(TEXT("%s/%s"), *Key, *Result);
	} else {
		LogMessage(TEXT("GI_Core could not be found!"), Warning);
	}
	
	return Result;
}

USaveGame* UAC_SaveSystem::getSaveGameFromPath(const FString path, TSubclassOf<USaveGame> type) {
	LogMessage("Function getSaveGameFromPath(): started...", ELogLevel::Verbose);
	if (!type) {
		throw UFL_Exception::CreateException(UEX_InvalidArgument::StaticClass(), FString::Printf(TEXT("getSaveGameFromPath(): type is invalid\n\t%s"), *path),this);
	}

	USaveGame* SG;
	if (UGameplayStatics::DoesSaveGameExist(path, 0)) {
		SG = UGameplayStatics::LoadGameFromSlot(path, 0);
	} else {
		SG = UGameplayStatics::CreateSaveGameObject(type);
	}
	if (SG) {
		LogMessage("Function getSaveGameFromPath(): return->SaveGameObject", ELogLevel::Verbose);
		return SG;
	}
	throw UFL_Exception::CreateException(UEX_InvalidArgument::StaticClass(), "getSaveGameFromPath(): type is invalid", this);
}