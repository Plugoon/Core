#pragma once

#include "CoreMinimal.h"
#include "Delegates.h"
#include "SaveSystem/I_SaveableCallable.h"
#include "E_LogLevel.h"
#include "I_Logger.h"
#include "GameFramework/SaveGame.h"
#include "GameFramework/PlayerController.h"
#include "SaveSystem/I_Saveable.h"

#include "PC_Core.generated.h"

UCLASS()
class CORESYSTEM_API APC_Core : public APlayerController, public II_Logger, public II_SaveableCallable, public II_Saveable {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Core|Logger", meta=(ToolTip="Enables Log to screen"))
    bool bDebug = false;
    UPROPERTY(EditAnywhere, Category = "Core|Logger", meta=(ToolTip="Log to screen duration"))
    float PrintTime = 5.f;
    UPROPERTY(EditAnywhere, Category= "Core|Logger", meta=(ToolTip="DebugLevel"))
    TEnumAsByte<ELogLevel> LogLevel = ELogLevel::Log;

    UPROPERTY(EditAnywhere, Category= "Core|Save", meta=(ToolTip="Should be Saved"))
    bool Savable;
    UPROPERTY(EditAnywhere, Category= "Core|Save", meta=(ToolTip="SaveGame Type"))
    TSubclassOf<USaveGame> Type;
    
    UPROPERTY(BlueprintAssignable, Category= "Core|Save")
    FVoidDelegate OnPlayerControllerLoaded;

protected:
    UPROPERTY(BlueprintReadWrite, Category="Core|Save")
    USaveGame* SaveGame;
    
private:
    FString DisplayName = "";

public:
    APC_Core();

    virtual void LogMessage(const FString& Message, ELogLevel Level) override;

protected:
    virtual void BeginPlay() override;
    virtual void Init();
    virtual void UpdateSaveGame() override;
    virtual void UpdateSaveGameImplementation_Implementation() override;
};
