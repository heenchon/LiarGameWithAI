// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameState.h"

#include "LiarPlayerState.h"
#include "EngineUtils.h"
#include "LiarGameWithAI/Chair.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void ALiarGameState::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AChair> It(GetWorld()); It; ++It)
	{
		Chairs.Add(*It);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%d"), Chairs.Num());
}

void ALiarGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// DOREPLIFETIME(ALiarGameState, LiarPlayer);
	// DOREPLIFETIME(ALiarGameState, PlayerList);
	// DOREPLIFETIME(ALiarGameState, TeleportPoints);
}

void ALiarGameState::LiarTest()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("서버 권한 잇음"));
		
		Multicast_GameStart();
	}
}

void ALiarGameState::Multicast_GameStart_Implementation()
{
	// 임시데이터

	for (int32 i = 0; i < 6; i++)
	{
		FPlayerInfo info = 
		{
			.id = FString::Printf(TEXT("Test%d"), i),
			.liar = i < 5,
			.order = i + 1
		};
		
		PlayerList.Add(info);
	}
	
	SortPlayer();
	InitPlayerInfo();
}

void ALiarGameState::SortPlayer()
{
	UE_LOG(LogTemp,Warning,TEXT("플레이어 정렬"));
	
	PlayerList.Sort([](const FPlayerInfo& A, const FPlayerInfo& B)
	{
		return A.order < B.order;
	});
}

void ALiarGameState::InitPlayerInfo()
{
	if (PlayerList.IsEmpty())
	{
		UE_LOG(LogTemp,Warning,TEXT("플레이어 리스트 Empty"));
		return;
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("플레이어 있음"));
	}

	// //TODO: 플레이어 Json 정보 받아서 정렬.
	//
	// if (TeleportPoints.IsEmpty())
	// {
	// 	UE_LOG(LogTemp,Warning,TEXT("텔레포트 포인트 없음"));
	// 	return;
	// }

	for (int i = 0; i < PlayerList.Num(); i++)
	{
		// TODO: playerList 아이디가 자신의 아이디와 같다면 PlayerController의 Pawn을 의자에 앉히자.

		FPlayerInfo info = PlayerList[i];
		
		Chairs[i]->SpawnPlayer(info.id, info.order, info.liar);
	}
}