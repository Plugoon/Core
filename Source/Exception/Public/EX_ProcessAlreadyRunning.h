#pragma once

#include "CoreMinimal.h"
#include "O_Exception.h"
#include "EX_ProcessAlreadyRunning.generated.h"

UCLASS()
class EXCEPTION_API UEX_ProcessAlreadyRunning : public UO_Exception {
	GENERATED_BODY()

	UEX_ProcessAlreadyRunning();
};
