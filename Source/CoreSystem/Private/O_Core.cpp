#include "O_Core.h"
#include "FL_Logger.h"

void UO_Core::LogMessage(const FString& Message, const ELogLevel Level) {
	if (DisplayName == "") {
		DisplayName = GetName();
	}

	if (Level >= LogLevel) {
		const FString LogMessage = FString::Printf(TEXT("%s: %s"), *DisplayName, *Message);
		UFL_Logger::OutputLog(Level, LogMessage);
		if (bDebug) {
			GEngine->AddOnScreenDebugMessage(-1, PrintTime, UFL_Logger::GetLogLevelColor(Level), LogMessage);
		}
	}
}
