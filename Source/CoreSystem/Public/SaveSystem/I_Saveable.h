#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "I_Saveable.generated.h"

UINTERFACE(MinimalAPI)
class UI_Saveable : public UInterface {
	GENERATED_BODY()
};

class CORESYSTEM_API II_Saveable {
	GENERATED_BODY()

public:
	/**
	* \brief Overwrite to implement class specific SaveGame update method.
	*/
	UFUNCTION(
		BlueprintNativeEvent,
		Category = "Core|SaveSystem",
		meta = (
			ToolTip = "Overwrite to implement class specific SaveGame update method."
		)
	)
    void UpdateSaveGameImplementation();

	/**
	* \brief Called when SaveGame was loaded.
	*/
	UFUNCTION(
		BlueprintImplementableEvent,
		Category = "Core|SaveSystem",
		meta = (
			ToolTip = "Called when SaveGame was loaded."
		)
	)
    void OnLoadFinished();

};
