#include "CM_Core.h"

#include "FL_Logger.h"

void UCM_Core::Pms_Logger_PrintTime(float Time) {
	const FString Message = FString::Printf(TEXT("Set Logger PrintTime to: %f"), Time);
	LogMessage(Message, ELogLevel::Warning);
	if (Time <= 0.0f) {
		PrintTime = 0.01f;
	}
	else {
		PrintTime = Time;
	}
}

void UCM_Core::Pms_Logger_LogLevel(uint8 Level) {
	const TEnumAsByte<ELogLevel> _LogLevel = TEnumAsByte<ELogLevel>(Level);
	const FString EnumName = UEnum::GetValueAsString<ELogLevel>(_LogLevel);
	const FString Message = FString::Printf(TEXT("Set Logger LogLevel to: %s"), *EnumName);
	LogLevel = (ELogLevel)Level;

	LogMessage(Message, ELogLevel::Warning);
}

void UCM_Core::LogMessage(const FString& Message, ELogLevel Level) {
	const FString DisplayName = "CheatManager";

	if (Level >= LogLevel) {
		const FString LogMessage = FString::Printf(TEXT("%s: %s"), *DisplayName, *Message);

		UFL_Logger::OutputLog(Level, LogMessage);

		GEngine->AddOnScreenDebugMessage(-1, PrintTime, UFL_Logger::GetLogLevelColor(Level), LogMessage);
	}
}
