// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "LiarGameState.generated.h"

class ALiarPlayerState;
/**
 * 
 */
UCLASS()
class LIARGAMEWITHAI_API ALiarGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated)
	TArray<ALiarPlayerState*> PlayerList;

	UPROPERTY(Replicated)
	ALiarPlayerState* LiarPlayer;
	
	UPROPERTY(Replicated)
	TArray<AActor*> TeleportPoints;


protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION(NetMulticast, reliable)
	void Multicast_GameStart();

	UFUNCTION()
	void SortPlayer();
	UFUNCTION()
	void InitPlayerInfo();
	
public:
	UFUNCTION(Exec)
	void LiarTest();
};
