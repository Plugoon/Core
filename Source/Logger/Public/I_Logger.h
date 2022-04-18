#pragma once

#include "CoreMinimal.h"
#include "E_LogLevel.h"
#include "UObject/Interface.h"
#include "UObject/ObjectMacros.h"


#include "I_Logger.generated.h"

UINTERFACE(MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UI_Logger : public UInterface {
	GENERATED_BODY()
};

class LOGGER_API II_Logger {
	GENERATED_BODY()

public:
	/**
	* \param Message Message to Log
	* \param Level Log Level
	*/
	UFUNCTION(
		BlueprintCallable,
		Category = "Core|Logger",
		meta=(
			ToolTip="Log Message\n\n@Message: Message to Log\n@Level: Log Level"
		)
	)
	virtual void LogMessage(const FString& Message, ELogLevel Level) {
	};
};
