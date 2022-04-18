#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "GI_Core.generated.h"

UCLASS()
class CORESYSTEM_API UGI_Core : public UGameInstance {
	GENERATED_BODY()

public:
	FString SaveGameKey = TEXT("C_Sharp_Mayor");

public:
	UGI_Core();
};
