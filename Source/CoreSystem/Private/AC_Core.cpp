#include "AC_Core.h"

#include "A_Core.h"
#include "C_Core.h"
#include "FL_Logger.h"
#include "SaveSystem/FL_SaveSystem.h"
#include "GM_Core.h"
#include "O_Exception.h"
#include "PC_Core.h"
#include "Kismet/KismetSystemLibrary.h"

UAC_Core::UAC_Core() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_Core::BeginPlay() {
	Super::BeginPlay();
}


void UAC_Core::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAC_Core::Init() {
	LogMessage("Init...", ELogLevel::Log);

	if (Savable) {
		LogMessage("IsSavable->getSaveGame", ELogLevel::Verbose);
		InternalBindToPrevSave();
		try {
			SaveGame = UFL_SaveSystem::GetSaveSystem()->GetSaveGame(this, Type);
		} catch (UO_Exception *ex) {
			LogMessage(ex->Message, Warning);
		}
	}
}

void UAC_Core::LogMessage(const FString& Message, ELogLevel Level) {
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

void UAC_Core::UpdateSaveGame() {
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

void UAC_Core::UpdateSaveGameImplementation_Implementation() {
	LogMessage(TEXT("UpdateSaveGameImplementation not overwritten in Blueprint!"), ELogLevel::Warning);
}

/* todo: Unreal uses same values when multiple components of the same type are on one actor
 *     -> name might be problematic
 *     - can and should we fix this?
 */
void UAC_Core::InternalBindToPrevSave() {
	TArray<UAC_Core*> Components;
	GetOwner()->GetComponents<UAC_Core>(Components);

	if (!PreviousComponent.Equals("None") && !PreviousComponent.Equals("")) {
		for (UAC_Core* Comp : Components) {
			if (Comp->GetName() == PreviousComponent && Comp->Savable) {
				if (Comp != this) {
					Comp->OnComponentLoaded.AddDynamic(this, &UAC_Core::OnLoad);
					return;
				}
				else {
					break;
				}
			}
		}
		const FString Message = FString::Printf(
			TEXT("InternalBindToPrevSave(): PreviousComponent(%s) not found->bind onLoad() to owner"),
			*PreviousComponent);
		LogMessage(Message, ELogLevel::Warning);
	}

	AA_Core* Actor = Cast<AA_Core>(GetOwner());
	if (Actor) {
		Actor->OnActorLoaded.AddDynamic(this, &UAC_Core::OnLoad);
		return;
	}

	APC_Core* PController = Cast<APC_Core>(GetOwner());
	if (PController) {
		PController->OnPlayerControllerLoaded.AddDynamic(this, &UAC_Core::OnLoad);
		return;
	}

	AC_Core* PCharacter = Cast<AC_Core>(GetOwner());
	if (PCharacter) {
		PCharacter->OnPlayerCharacterLoaded.AddDynamic(this, &UAC_Core::OnLoad);
		return;
	}

	AGM_Core* GameMode = Cast<AGM_Core>(GetOwner());
	if (GameMode) {
		GameMode->OnGameModeLoaded.AddDynamic(this, &UAC_Core::OnLoad);
		return;
	}

	LogMessage("InternalBindToPrevSave(): Something went wrong->OnLoad() wont be called!", ELogLevel::Warning);
}

void UAC_Core::OnLoad() {
	Execute_OnLoadFinished(this);
	OnComponentLoaded.Broadcast();
}
