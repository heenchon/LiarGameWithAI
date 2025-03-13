// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameState.h"

#include "GameFramework/Character.h"
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
}

void ALiarGameState::Test()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Display, TEXT("서버 권한 잇음"));
		Multicast_GameStart();
	}
}

void ALiarGameState::Multicast_GameStart_Implementation()
{
	Multicast_ChooseLiar();
	Multicast_ShowKeyword();
}

// void ALiarGameState::GameStart()
// {
// 	
// }

void ALiarGameState::Multicast_InitPlayers_Implementation(const TArray<APlayerController*>& players)
{
	PlayerNum = players.Num();
	PlayerList.SetNum(PlayerNum);
	
	for (int i = 0; i < PlayerList.Num(); i++)
	{
		PlayerList[i] = players[i];
		// Players[i]->SetActorLocation(TeleportPoints[i]->GetActorLocation());
		// Players[i]->SetActorRotation(TeleportPoints[i]->GetActorRotation());
		//TODO: 플레이어 앉은 포즈 시작
	}
}

void ALiarGameState::Multicast_ChooseLiar_Implementation()
{
	int randPlayer = FMath::RandRange(0, PlayerNum - 1);
	LiarPlayer = PlayerList[randPlayer];
}

void ALiarGameState::Multicast_ShowKeyword_Implementation()
{
	for (APlayerController* player : PlayerList)
	{
		player->GetCharacter()->SetActorRotation(FRotator(0, 0, -180));
	}
}
