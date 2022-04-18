#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "I_SaveSystemListener.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UI_SaveSystemListener : public UInterface {
	GENERATED_BODY()
};

class CORESYSTEM_API II_SaveSystemListener {
	GENERATED_BODY()

	public:
	/**
	* \brief Called when SaveGame was loaded.
	*/
	UFUNCTION(
        BlueprintNativeEvent,
        Category = "Core|SaveSystem",
        meta = (
            ToolTip = "Called when Saving is done."
        )
    )
    void OnSaveFinished();

};
