#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/ObjectMacros.h"

#include "I_SaveableCallable.generated.h"

UINTERFACE(MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UI_SaveableCallable : public UInterface {
	GENERATED_BODY()
};

class CORESYSTEM_API II_SaveableCallable {
	GENERATED_BODY()
    
public:
    /**
    * \brief Call to Update the SaveGame.
    * Will also prepare the SaveGame to be saved to disk at the next save.
    */
    UFUNCTION(
        BlueprintCallable,
        Category = "Core|SaveSystem",
        meta=(
            ToolTip="Call to Update the SaveGame.\nWill also prepare the SaveGame to be saved to disk at the next save."
            )
        )
    virtual void UpdateSaveGame(){};
};