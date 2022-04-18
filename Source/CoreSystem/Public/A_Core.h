#pragma once

#include "CoreMinimal.h"
#include "Delegates.h"
#include "I_Logger.h"
#include "SaveSystem/I_Saveable.h"
#include "SaveSystem/I_SaveableCallable.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"


#include "A_Core.generated.h"

UCLASS()
class CORESYSTEM_API AA_Core : public AActor, public II_Logger, public II_SaveableCallable, public II_Saveable {
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
	bool Savable;
	UPROPERTY(EditAnywhere, Category= "Core|Save", meta=(ToolTip="SaveGame Type"))
	TSubclassOf<USaveGame> Type;
	
	UPROPERTY(BlueprintAssignable, Category= "Core|Save")
	FVoidDelegate OnActorLoaded;

protected:
	UPROPERTY(BlueprintReadWrite, Category="Core|Save")
	USaveGame* SaveGame;

private:
	FString DisplayName = "";

public:
	// Sets default values for this actor's properties
	AA_Core();
	virtual void LogMessage(const FString& Message, ELogLevel Level) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void Init();
	virtual void UpdateSaveGame() override;
	virtual void UpdateSaveGameImplementation_Implementation() override;
};
