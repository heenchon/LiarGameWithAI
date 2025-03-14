#pragma once

#include "CoreMinimal.h"
#include "LiarGameInfo.generated.h"

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


// 게임 시작시 기본적으로 초기화해야하는 데이터들
USTRUCT()
struct FGameInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FPlayerInfo> players;
	UPROPERTY()
	FString common_keyword;
	UPROPERTY()
	FString lair_keyword;
};
