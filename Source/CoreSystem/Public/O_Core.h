#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "E_LogLevel.h"
#include "I_Logger.h"

#include "O_Core.generated.h"

UCLASS()
class CORESYSTEM_API UO_Core : public UObject, public II_Logger
{
	GENERATED_BODY()

public:
	//Logger Properties
	UPROPERTY(EditAnywhere, Category = "Core|Logger", meta=(ToolTip="Enables Log to screen"))
    bool bDebug = false;
	UPROPERTY(EditAnywhere, Category = "Core|Logger", meta=(ToolTip="Log to screen duration"))
    float PrintTime = 5.f;
	UPROPERTY(EditAnywhere, Category= "Core|Logger", meta=(ToolTip="DebugLevel"))
    TEnumAsByte<ELogLevel> LogLevel = ELogLevel::Log;

private:
	FString DisplayName = "";

public:
	virtual void LogMessage(const FString& Message, ELogLevel Level) override;
};
