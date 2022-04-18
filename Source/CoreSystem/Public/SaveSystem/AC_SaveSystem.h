#pragma once

#include "CoreMinimal.h"
#include "I_Logger.h"
#include "I_SaveSystem.h"
#include "I_SaveSystemListener.h"
#include "Runnable_Save.h"
#include "S_Exception.h"
#include "Components/ActorComponent.h"
#include "GameFramework/SaveGame.h"

#include "AC_SaveSystem.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CORESYSTEM_API UAC_SaveSystem : public UActorComponent, public II_Logger, public I_AsyncSavingCallback, public II_SaveSystem {
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category="Debug|Config")
	bool bDebug;
	UPROPERTY(EditAnywhere, Category = "Core|Logger", meta=(ToolTip="Log to screen duration"))
	float PrintTime = 5.f;
	UPROPERTY(EditAnywhere, Category= "Core|Logger", meta=(ToolTip="DebugLevel"))
	TEnumAsByte<ELogLevel> LogLevel = ELogLevel::Log;

private:
	FString DisplayName;
	FString ActorComponentDisplayName;

	UPROPERTY()
	TMap<FString, USaveGame*> AllSaveGames;
	UPROPERTY()
	TMap<FString, USaveGame*> ChangedSaveGames;
	UPROPERTY()
	TMap<FString, USaveGame*> SavingSaveGames;
	Runnable_Save* RunnableTask;
	UPROPERTY()
	TArray<TScriptInterface<II_SaveSystemListener>> Listeners;

public:
	// Sets default values for this component's properties
	UAC_SaveSystem();
	
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	virtual void LogMessage(const FString& Message, ELogLevel Level) override;

	virtual void SaveAll() override;
	virtual void SaveAll(FS_Exception &Exception) override;
	virtual USaveGame* GetSaveGame(UObject* Source, TSubclassOf<USaveGame> Type) override;
	virtual bool IsCurrentlySaving() override;
	virtual void UpdateSaveGame(UObject* Source, USaveGame* SaveGame) override;
	virtual void Subscribe(TScriptInterface<II_SaveSystemListener> Listener) override;
	virtual void Unsubscribe(TScriptInterface<II_SaveSystemListener> Listener) override;

	// called from saving thread
	virtual void OnSaveFinished() override;

protected:
	virtual void BeginPlay() override;

private:
	FString getSaveGamePath(UObject* source);
	USaveGame* getSaveGameFromPath(FString path, TSubclassOf<USaveGame> type);
};
