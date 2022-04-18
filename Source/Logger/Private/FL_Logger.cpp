#include "FL_Logger.h"

DEFINE_LOG_CATEGORY(LogGame);

FColor UFL_Logger::GetLogLevelColor(const ELogLevel &Level) {
    switch (Level) {
    case ELogLevel::Log:
        return FColor::Blue;
    case ELogLevel::Warning:
        return FColor::Yellow;
    case ELogLevel::Error:
        return FColor::Red;
    case ELogLevel::Fatal:
        return FColor::Black;
    default:
        return FColor::White;
    }
}

void UFL_Logger::OutputLog(const ELogLevel &Level, const FString &LogMessage) {
    switch (Level) {
    case VeryVerbose:
    UE_LOG(LogGame, VeryVerbose, TEXT("%s"), *LogMessage);
        break;
    case Verbose:
    UE_LOG(LogGame, Verbose, TEXT("%s"), *LogMessage);
        break;
    case Log:
    UE_LOG(LogGame, Log, TEXT("Log: %s"), *LogMessage);
        break;
    case Warning:
    UE_LOG(LogGame, Warning, TEXT("%s"), *LogMessage);
        break;
    case Error:
    UE_LOG(LogGame, Error, TEXT("%s"), *LogMessage);
        break;
    case Fatal:
    UE_LOG(LogGame, Fatal, TEXT("%s"), *LogMessage);
        break;
    default:
        break;
    }
}