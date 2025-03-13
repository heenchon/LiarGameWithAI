// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "LiarGameState.generated.h"

/**
 * 
 */
UCLASS()
class LIARGAMEWITHAI_API ALiarGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated)
	TArray<APlayerController*> PlayerList;

	UPROPERTY(Replicated)
	APlayerController* LiarPlayer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> TeleportPoints;


protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION(NetMulticast, reliable)
	void Multicast_GameStart();

	UFUNCTION(NetMulticast, reliable)
	void Multicast_InitPlayers(const TArray<APlayerController*>& players);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ChooseLiar();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ShowKeyword();

	
public:
	UFUNCTION(Exec)
	void Test();
};
