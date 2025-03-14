﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameState.h"

#include "LiarPlayerState.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "LiarGameWithAI/Chair.h"

void ALiarGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ALiarGameState::LiarTest()
{
	// 임시데이터

	TArray<FPlayerInfo> Players;
	
	for (int32 i = 0; i < 3; i++)
	{
		FPlayerInfo info = 
		{
			.id = FString::Printf(TEXT("Test%d"), i),
			.liar = i < 1,
			.order = i + 1
		};
		
		Players.Add(info);
	}
	
	// TestInfo.players = Players;
	// TestInfo.common_keyword = TEXT("딸기");
	// TestInfo.lair_keyword = TEXT("바나나");

	GameStart();
}

void ALiarGameState::GameStart()
{
	InitPlayer();
	// InitKeyword(TestInfo.common_keyword, TestInfo.lair_keyword);
	ShowKeyword();
	Round();
}

void ALiarGameState::Round()
{
	if (CurRound == 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("게임 끝!"));
		CurrentOrder = 0;
		CurRound = 0;
		
		VotingStart();
		return;
	}
	
	if (CurrentOrder == 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("다음 라운드!"));
		CurrentOrder = 0;
		ShowKeyword();
		
		++CurRound;
	}
		
	// order에 해당하는 Player를 향해 카메라를 돌린다
	UE_LOG(LogTemp, Warning, TEXT("id: %s"), *PlayerList[CurrentOrder].id);

	UpdateCameraByOrder(CurrentOrder);
	CollectAnswers(CurrentOrder);
	
	++CurrentOrder;
}

void ALiarGameState::InitPlayer()
{
	if (PlayerList.IsEmpty())
	{
		UE_LOG(LogTemp,Warning,TEXT("플레이어 리스트 Empty"));
		return;
	}

	for (TActorIterator<AChair> It(GetWorld()); It; ++It)
	{
		Chairs.Add(*It);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%d"), Chairs.Num());

	// 게임의 전체를 비춰주는 카메라 찾기
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		CameraActor = Cast<ACameraActor>(PC->GetViewTarget());
	}

	ACharacter* My = GetWorld()->GetFirstPlayerController()->GetCharacter();
	ALiarGameWithAICharacter* MyPlayer = Cast<ALiarGameWithAICharacter>(My);

	for (int i = 0; i < PlayerList.Num(); i++)
	{
		// playerList 아이디가 자신의 아이디와 같다면 PlayerController의 Pawn을 의자에 앉히자.

		FPlayerInfo info = PlayerList[i];

		if (MyPlayer->UserId == info.id)
		{
			IamLiar = info.liar;
			MyPlayer->SetActorTransform(Chairs[i]->SitPosition->GetComponentTransform());
			MyPlayer->SetSitAnim();
			continue;
		}
	
		Chairs[i]->SpawnPlayer(info.id, info.order, info.liar);
	}
}

void ALiarGameState::InitKeyword(const FString& common, const FString& lair)
{
	CommonKeyword = common;
	LairKeyword = lair;
}

void ALiarGameState::ShowKeyword()
{
	// 나의 아이디를 가지고 PlayerList에서 정보를 찾는다
	// 내가 라이어이면 라이어키워드를 출력

	FString value = IamLiar? LairKeyword : CommonKeyword;

	FString Msg = FString::Printf(TEXT("내 키워드: %s"), *value);
	ScreenLog(Msg);
}

void ALiarGameState::UpdateCameraByOrder(int order)
{
	AChair* curChair = Chairs[order];
	FVector chairLoc = curChair->GetActorLocation();
	FRotator chairRot = curChair->GetActorRotation();
	
	UE_LOG(LogTemp, Warning, TEXT("순서에 해당하는 의자: %s"), *curChair->GetActorNameOrLabel());
	
	FVector newLoc = FVector(0,0,110);
	FRotator newRot = chairRot+ FRotator(0,360-60*CurrentOrder,0); 
	
	//UE_LOG(LogTemp, Warning, TEXT("위치: %s"), *newLoc.ToString());;
	//UE_LOG(LogTemp, Warning, TEXT("회전: %s"), *newRot.ToString());;
	
	CameraActor->SetActorLocation(newLoc);
	CameraActor->SetActorRotation(newRot);
}

void ALiarGameState::CollectAnswers(int order)
{
	ACharacter* My = GetWorld()->GetFirstPlayerController()->GetCharacter();
	ALiarGameWithAICharacter* MyPlayer = Cast<ALiarGameWithAICharacter>(My);

	// 현재 의자에 앉아있는 유저 id가 내 id가 같다면, 입력
	
	if (MyPlayer->UserId == PlayerList[CurrentOrder].id)
	{
		InputAnswer();
	}
	else if (CurrentOrder == 2)
	{
		//TODO: ai한테 제시어 받기
	}
	else
	{
		WaitingOthersAnswer();
	}
}

void ALiarGameState::InputAnswer()
{
	ScreenLog("Type short explanation of the keyword");
}

void ALiarGameState::WaitingOthersAnswer()
{
	//TODO: 다른 플레이어가 연관어 말했는지 확인하기
	ScreenLog("Waiting for Others Answer...");
}

void ALiarGameState::VotingStart()
{
	CameraActor->SetActorLocation(FVector(0,0,0));
	CameraActor->SetActorRotation(FRotator(0,-90,0));
	CollectVotes();
}

void ALiarGameState::CollectVotes()
{
	++CurrentOrder;
	if (CurrentOrder == 3)
	{
		ScreenLog("투표 끝!");
		VotingEnd();
		return;
	}
	
	ACharacter* My = GetWorld()->GetFirstPlayerController()->GetCharacter();
	ALiarGameWithAICharacter* MyPlayer = Cast<ALiarGameWithAICharacter>(My);
	
	if (MyPlayer->UserId == PlayerList[CurrentOrder].id)
	{
		Vote();
	}
	else if (CurrentOrder == 2)
	{
		//TODO:ai 투표 받기
	}
	else
	{
		WaitingOthersVote();
	}
}

void ALiarGameState::Vote()
{
	//TODO: 투표하기
}

void ALiarGameState::WaitingOthersVote()
{
	//TODO: 다른 사람 투표 입력 기다리기
}

void ALiarGameState::VotingEnd()
{
	//TODO: 뽑힌 플레이어 정보 가져오기
	//UE_LOG(LogTemp,Warning,TEXT("뽑힌 플레이어: %s"), *);
}

void ALiarGameState::ScreenLog(const FString& string)
{
	FString Msg = FString::Printf(TEXT("%s"), *string);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Msg);
}

void ALiarGameState::Initialize(const FGameInfo& info)
{
	// TODO: ChatManager에게 게임의 정보를 요청하자
	
	// 받은 데이터를 가지고 매니저 initialize하자
	// 라운드 시작하자 -> 1번째부터 시작!
	
	for (int32 i = 0; i < info.players.Num(); i++)
	{
		PlayerList.Add(info.players[i]);
	}
	
	PlayerList.Sort([](const FPlayerInfo& A, const FPlayerInfo& B)
	{
		return A.order < B.order;
	});
	
	InitKeyword(info.common_keyword, info.lair_keyword);
	GameStart();
}
