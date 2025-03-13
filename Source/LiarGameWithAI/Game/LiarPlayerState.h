// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LiarPlayerState.generated.h"

/**
 * 
 */
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

UCLASS()
class LIARGAMEWITHAI_API ALiarPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Info")
	FPlayerInfo playerInfo;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
};
