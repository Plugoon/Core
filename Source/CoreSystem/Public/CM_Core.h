#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "E_LogLevel.h"
#include "I_Logger.h"

#include "CM_Core.generated.h"

UCLASS()
class CORESYSTEM_API UCM_Core : public UCheatManager, public II_Logger{
	GENERATED_BODY()

private:
	float PrintTime = 5.f;
    TEnumAsByte<ELogLevel> LogLevel = ELogLevel::Log;

public:
	UFUNCTION(Exec)
	void Pms_Logger_PrintTime(float Time);
	UFUNCTION(Exec)
	void Pms_Logger_LogLevel(uint8 Level);

private:
	void LogMessage(const FString& Message, ELogLevel Level) override;
};
