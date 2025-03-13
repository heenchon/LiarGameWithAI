// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameState.h"

#include "EngineUtils.h"
#include "LiarGameWithAI/Chair.h"
#include "GameFramework/Character.h"
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
	
	DOREPLIFETIME(ALiarGameState, LiarPlayer);
	DOREPLIFETIME(ALiarGameState, PlayerList);
}

void ALiarGameState::Test()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("서버 권한 잇음"));
		Multicast_GameStart();
	}
}

void ALiarGameState::Multicast_GameStart_Implementation()
{
	Multicast_ChooseLiar();
	//Multicast_ShowKeyword();
}

// void ALiarGameState::GameStart()
// {
// 	
// }

void ALiarGameState::Multicast_InitPlayers_Implementation(const TArray<APlayerController*>& players)
{
	PlayerList.SetNum(players.Num());
	
	for (int i = 0; i < PlayerList.Num(); i++)
	{
		PlayerList[i] = players[i];

		players[i]->GetCharacter()->SetActorTransform(Chairs[i]->SitPosition->GetComponentTransform());
		
		// Players[i]->SetActorLocation(TeleportPoints[i]->GetActorLocation());
		// Players[i]->SetActorRotation(TeleportPoints[i]->GetActorRotation());
		//TODO: 플레이어 앉은 포즈 시작
	}
}

void ALiarGameState::Multicast_ChooseLiar_Implementation()
{
	if (PlayerList.IsEmpty())
	{
		UE_LOG(LogTemp,Warning,TEXT("플레이어 리스트 Empty"));
		return;
	}
	
	int randPlayer = FMath::RandRange(0, PlayerList.Num() - 1);
	LiarPlayer = PlayerList[randPlayer];
}

void ALiarGameState::Multicast_ShowKeyword_Implementation()
{
	for (APlayerController* player : PlayerList)
	{
		player->GetCharacter()->SetActorRotation(FRotator(0, 0, -180));
	}
}
