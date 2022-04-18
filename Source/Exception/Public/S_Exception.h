#pragma once

#include "CoreMinimal.h"
#include "S_Exception.generated.h"

USTRUCT(BlueprintType)
struct FS_Exception {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bHasError;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ExceptionType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Message;
};
