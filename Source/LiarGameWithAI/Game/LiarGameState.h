// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LiarPlayerState.h"
#include "GameFramework/GameState.h"
#include "LiarGameState.generated.h"

class AChatManager;
class ALiarPlayerState;
/**
 * 
 */
UCLASS()
class LIARGAMEWITHAI_API ALiarGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	AChatManager* ChatManager;
	
	// UPROPERTY(Replicated)
	TArray<FPlayerInfo> PlayerList;
	
	UPROPERTY(Replicated)
	TArray<ALiarPlayerState*> PlayerStates;
	
	// UPROPERTY(Replicated)
	// ALiarPlayerState* LiarPlayer;

	UPROPERTY()
	TArray<class AChair*> Chairs;

	FString common_keyword = "common";
	FString liar_keyword = "liar";
	
	int CurRound = 0;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION(NetMulticast, reliable)
	void Multicast_GameStart();
	UFUNCTION(NetMulticast, reliable)
	void Multicast_Round();

	UFUNCTION()
	void SortPlayer();
	UFUNCTION()
	void InitPlayer();
	void InitKeyword();
	void ShowKeyword();

	void CollectAnswers(int order);
	void InputAnswer();
	void WaitingOthersAnswer();
	
	void ScreenLog(const FString& string);
	
public:
	UFUNCTION(Exec)
	void LiarTest();
};
