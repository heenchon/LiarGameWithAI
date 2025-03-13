// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameState.h"

#include "LiarPlayerState.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void ALiarGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ALiarGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ALiarGameState, LiarPlayer);
	DOREPLIFETIME(ALiarGameState, PlayerList);
	DOREPLIFETIME(ALiarGameState, TeleportPoints);
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
	TeleportPoints.SetNum(6);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(),FName("Point"),TeleportPoints);
	
	SortPlayer();
	InitPlayerInfo();
}

void ALiarGameState::SortPlayer()
{
	UE_LOG(LogTemp,Warning,TEXT("플레이어 정렬"));
	
	PlayerList.Sort([](const ALiarPlayerState& A, const ALiarPlayerState& B)
	{
		return A.GetPlayerId() < B.GetPlayerId();
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

	//TODO: 플레이어 Json 정보 받아서 정렬.

	if (TeleportPoints.IsEmpty())
	{
		UE_LOG(LogTemp,Warning,TEXT("텔레포트 포인트 없음"));
		return;
	}

	for (int i = 0; i < PlayerList.Num(); i++)
	{
		PlayerList[i]->GetPlayerController();
		PlayerList[i]->GetPlayerController();
	}
}

void ALiarGameState::Multicast_InitPlayers_Implementation(const TArray<APlayerController*>& players)
{
	PlayerList.SetNum(players.Num());
	
	for (int i = 0; i < PlayerList.Num(); i++)
	{
		PlayerList[i] = Cast<ALiarPlayerState>(players[i]->PlayerState);
		
		// Players[i]->SetActorLocation(TeleportPoints[i]->GetActorLocation());
		// Players[i]->SetActorRotation(TeleportPoints[i]->GetActorRotation());
		//TODO: 플레이어 앉은 포즈 시작
	}
}