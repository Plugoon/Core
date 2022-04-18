#pragma once

#include "SaveSystem/AC_SaveSystem.h"
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "FL_SaveSystem.generated.h"

UCLASS()
class CORESYSTEM_API UFL_SaveSystem : public UBlueprintFunctionLibrary {
	GENERATED_BODY()
	
public:
	/**
	 * \brief Check if source is a Core Class
	 * \param source 
	 * \return true if source is Core Class
	 */
	UFUNCTION(
        BlueprintPure,
        Category="Core|SaveSystem",
        meta=(
            ToolTip="Check if source is a Core Class\n\n@source: Object to check\n\n Return: true if source is Core Class"
            )
        )
    static bool IsCoreClass(UObject* source);

	/**
	 * \brief Get the SaveSystem
	 * \return SaveSystem
	 */
	UFUNCTION(
        BlueprintPure,
        Category="Core|SaveSystem",
        meta=(
            ToolTip="Get the SaveSystem.\n\n Return: Save System"
            )
        )
	static TScriptInterface<II_SaveSystem> GetSaveSystem();
};
