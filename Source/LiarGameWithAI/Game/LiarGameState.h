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
	ACameraActor* CameraActor;
	
	UPROPERTY()
	AChatManager* ChatManager;

	// FGameInfo TestInfo;
	
	TArray<FPlayerInfo> PlayerList;

	UPROPERTY()
	TArray<class AChair*> Chairs;

	FString CommonKeyword = "common";
	FString LairKeyword = "liar";

	// 내가 라이어인가?
	bool IamLiar = false;
	
	int32 CurRound = 0;
	int32 CurrentOrder = 0;

	TArray<FString> Answers; // 그동안 말한 단어 저장용

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void GameStart();
	
	UFUNCTION()
	void InitPlayer();
	void InitKeyword(FString common, FString liar);
	void ShowKeyword();
	
	void UpdateCameraByOrder(int order);
	void CollectAnswers(int order);
	void InputAnswer();
	void WaitingOthersAnswer();

	void VotingStart();
	void CollectVotes();
	void Vote();
	void WaitingOthersVote();
	void VotingEnd();
	
	void ScreenLog(const FString& string);
	
public:
	void Initialize(FGameInfo info);
	
	UFUNCTION(Exec)
	void LiarTest();
	UFUNCTION(Exec)
	void Round();
};
