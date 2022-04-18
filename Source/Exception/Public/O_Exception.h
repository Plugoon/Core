#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "S_Exception.h"

#include "O_Exception.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class EXCEPTION_API UO_Exception : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ExceptionType = "Exception";
	UPROPERTY(BlueprintReadOnly)
	FString Message;

	FS_Exception ConvertToStruct() const;
};
