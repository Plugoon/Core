#pragma once

#include "CoreMinimal.h"
#include "Delegates.h"
#include "I_Logger.h"
#include "SaveSystem/I_Saveable.h"
#include "SaveSystem/I_SaveableCallable.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"

#include "AC_Core.generated.h"

UCLASS(ClassGroup = (Custom), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))

class CORESYSTEM_API UAC_Core : public UActorComponent, public II_Logger, public II_SaveableCallable, public II_Saveable {
	GENERATED_BODY()

public:
	//Logger Properties
	UPROPERTY(EditAnywhere, Category = "Core|Logger", meta=(ToolTip="Enables Log to screen"))
	bool bDebug = false;
	UPROPERTY(EditAnywhere, Category = "Core|Logger", meta=(ToolTip="Log to screen duration"))
	float PrintTime = 5.f;
	UPROPERTY(EditAnywhere, Category= "Core|Logger", meta=(ToolTip="DebugLevel"))
	TEnumAsByte<ELogLevel> LogLevel = ELogLevel::Log;

	//SaveSystem Properties
	UPROPERTY(EditAnywhere, Category= "Core|Save", meta=(ToolTip="Should be Saved"))
	bool Savable = false;
	UPROPERTY(EditAnywhere, Category= "Core|Save", meta=(ToolTip="SaveGame Type"))
	TSubclassOf<USaveGame> Type;
	UPROPERTY(EditAnywhere, Category="Core|Save", meta=(ToolTip="Waits on Previous Component before loading, None = Loads with BeginPlay"))
	FString PreviousComponent = "None";

	UPROPERTY(BlueprintAssignable, Category= "Core|Save")
	FVoidDelegate OnComponentLoaded;

protected:
	UPROPERTY(BlueprintReadWrite, Category="Core|Save")
	USaveGame* SaveGame;

private:
	FString DisplayName = "";

public:
	// Sets default values for this component's properties
	UAC_Core();
	virtual void LogMessage(const FString& Message, ELogLevel Level) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void Init();

	virtual void UpdateSaveGame() override;
	virtual void UpdateSaveGameImplementation_Implementation() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void InternalBindToPrevSave();
	UFUNCTION()
	void OnLoad();
};
