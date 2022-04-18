#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"

UENUM()
enum ELogLevel {
	VeryVerbose UMETA(DisplayName = "VeryVerbose"),
	Verbose UMETA(DisplayName = "Verbose"),
	Log UMETA(DisplayName = "Log"),
	Warning UMETA(DisplayName = "Warning"),
	Error UMETA(DisplayName = "Error"),
	Fatal UMETA(DisplayName = "Fatal"),
};
