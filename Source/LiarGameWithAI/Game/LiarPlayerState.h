// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LiarGameWithAI/LiarGameInfo.h"
#include "LiarPlayerState.generated.h"

/**
 * 
 */

UCLASS()
class LIARGAMEWITHAI_API ALiarPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, VisibleAnywhere, Category="Info")
	FPlayerInfo playerInfo;

	UPROPERTY(Replicated, VisibleAnywhere)
	FString Keyword;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
