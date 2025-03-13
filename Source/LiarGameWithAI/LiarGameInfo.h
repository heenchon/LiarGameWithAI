#pragma once

#include "CoreMinimal.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(visibleAnywhere)
	FString id;
	UPROPERTY(visibleAnywhere)
	bool liar;
	UPROPERTY(visibleAnywhere)
	int32 order;
};