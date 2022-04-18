#pragma once

#include "CoreMinimal.h"
#include "I_SaveSystemListener.h"
#include "S_Exception.h"
#include "GameFramework/SaveGame.h"
#include "I_SaveSystem.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint), BlueprintType)
class UI_SaveSystem : public UInterface {
	GENERATED_BODY()
};

class CORESYSTEM_API II_SaveSystem {
	GENERATED_BODY()

public:
	UFUNCTION(
        BlueprintCallable,
        Category="Core|SaveSystem",
        meta=(
            ToolTip="Save all saveable Objects in current Level\nReturn: was saving started successfully"
            )
        )
	virtual void SaveAll(FS_Exception &Exception) {}

	/**
	* \brief Save all saveable Objects in current Level
	* \return saving started successfully
	*/
	virtual void SaveAll() {}

	/**
	* \brief Loads the SaveGame or creates a new one if none was found
	* \param Source owner of the SaveGame
	* \param Type save game type, get's used when a new save game object has to be created
	*/
	virtual USaveGame* GetSaveGame(UObject* Source, TSubclassOf<USaveGame> Type) {return nullptr;}

	/**
	* \brief Check if a save process is currently running
	* \return true if running
	*/
	UFUNCTION(
        BlueprintCallable,
        Category="Core|SaveSystem",
        meta=(
            ToolTip="Check if a save process is currently running"
            )
        )
	virtual bool IsCurrentlySaving() {return false;}

	/**
	* \brief Update given SaveGame object
	* \param Source object that calls the method
	* \param SaveGame updated SaveGame object
	*/
	virtual void UpdateSaveGame(UObject* Source, USaveGame* SaveGame) {}

	/**
	* \brief Subscribe to the save system.
	* \param Listener Listener Object that will be subscribed
	*/
	UFUNCTION(
        BlueprintCallable,
        Category="Core|SaveSystem",
        meta=(
            ToolTip="Subscribe to the save system.\n@Listener: Listener Object that will be subscribed"
            )
        )
	virtual void Subscribe(TScriptInterface<II_SaveSystemListener> Listener) {}

	/**
	* \brief Unsubscribe from the save system.
	* \param Listener Listener Object that will be unsubscribed
	*/
	UFUNCTION(
        BlueprintCallable,
        Category="Core|SaveSystem",
        meta=(
            ToolTip="Unsubscribe from the save system.\n@Listener: Listener Object that will be unsubscribed"
            )
        )
	virtual void Unsubscribe(TScriptInterface<II_SaveSystemListener> Listener) {}
};
