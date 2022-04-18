#pragma once

#include "CoreMinimal.h"
#include "Delegates.h"
#include "E_LogLevel.h"
#include "I_Logger.h"
#include "SaveSystem/I_Saveable.h"
#include "SaveSystem/I_SaveableCallable.h"
#include "GameFramework/Character.h"
#include "GameFramework/SaveGame.h"

#include "C_Core.generated.h"

UCLASS()
class CORESYSTEM_API AC_Core : public ACharacter, public II_Logger, public II_SaveableCallable, public II_Saveable {
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
	FVoidDelegate OnPlayerCharacterLoaded;

protected:
	UPROPERTY(BlueprintReadWrite, Category="Core|Save")
	USaveGame* SaveGame;

private:
	FString DisplayName = "";

public:
	AC_Core();
	virtual void Tick(float DeltaTime) override;

	virtual void LogMessage(const FString& Message, ELogLevel Level) override;

	/**
	* \brief Spawn a new player character, possess him and delete the old one.
	* \param _Type Class of the spawned player character
	* \param _Transform Spawn transform
	* \param _Owner The Actor that spawned this Actor (Can be left as NULL)
	* \param _Instigator The APawn that is responsible for damage done by the spawned Actor
	* \param _CollisionHandling Method for resolving collisions at the spawn point
	* \return Returns: new player character
	*/
	UFUNCTION(
		BlueprintCallable,
		Category="Core|SaveSystem",
		meta=(
			Tooltip=
			"Spawn a new player character, possess him and delete the old one.\n\n@Type: Class of the spawned player character\n@Transform: Spawn transform\n@Owner: The Actor that spawned this Actor (Can be left as NULL)\n@Instigator: The APawn that is responsible for damage done by the spawned Actor\n@CollisionHandling: Method for resolving collisions at the spawn point\n\n@Returns: new player character"
		)
	)
	static AC_Core* ReplacePlayerCharacter(TSubclassOf<AC_Core> _Type, const FTransform& _Transform, AActor* _Owner,
	                                       APawn* _Instigator, ESpawnActorCollisionHandlingMethod _CollisionHandling);

protected:
	virtual void BeginPlay() override;
	virtual void Init();
	virtual void UpdateSaveGame() override;
	virtual void UpdateSaveGameImplementation_Implementation() override;
};
