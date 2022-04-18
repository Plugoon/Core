#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "O_Exception.h"
#include "S_Exception.h"

#include "FL_Exception.generated.h"

UCLASS()
class EXCEPTION_API UFL_Exception : public UBlueprintFunctionLibrary {
	GENERATED_BODY()
	
public:
	UFUNCTION(
		BlueprintPure,
		Category="Core|Exception",
		meta=(
			ToolTip=
			"Creates an Exception\n\n@Type: Exception Type\n@Message: Exception Message\n@Owner: Calling Object\n\nReturns Exception: Created Exception"
		)
	)
	static void CreateException(TSubclassOf<UO_Exception> Type, const FString& Message, UObject* Owner,
								FS_Exception& Exception);

	/**
	* \brief Creates an Exception
	* \param Type Exception Type
	* \param Message Exception Message
	* \param Owner Calling Object
	* \return Created Exception
	*/
	static UO_Exception* CreateException(TSubclassOf<UO_Exception> Type, const FString& Message, UObject* Owner);
};
