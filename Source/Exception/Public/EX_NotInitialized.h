#pragma once

#include "CoreMinimal.h"
#include "O_Exception.h"
#include "EX_NotInitialized.generated.h"

UCLASS()
class EXCEPTION_API UEX_NotInitialized : public UO_Exception {
	GENERATED_BODY()

	UEX_NotInitialized();
};
