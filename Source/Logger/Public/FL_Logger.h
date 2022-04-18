#pragma once

#include "CoreMinimal.h"
#include "E_LogLevel.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "FL_Logger.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGame, VeryVerbose, All);

UCLASS()
class LOGGER_API UFL_Logger : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	static FColor GetLogLevelColor(const ELogLevel& Level);
	static void OutputLog(const ELogLevel& Level, const FString& LogMessage);
};
