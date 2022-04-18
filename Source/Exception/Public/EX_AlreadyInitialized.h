#pragma once

#include "CoreMinimal.h"
#include "O_Exception.h"
#include "EX_AlreadyInitialized.generated.h"

UCLASS()
class EXCEPTION_API UEX_AlreadyInitialized : public UO_Exception {
	GENERATED_BODY()

	UEX_AlreadyInitialized();
};
