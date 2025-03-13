// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameSystem.h"

#include "GameFramework/Character.h"


ULiarGameSystem::ULiarGameSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void ULiarGameSystem::BeginPlay()
{
	Super::BeginPlay();
}

void ULiarGameSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void ULiarGameSystem::InitPlayers(TArray<ACharacter*> players)
{
	PlayerNum = players.Num();
	Players.SetNum(PlayerNum);
	
	for (int i = 0; i < Players.Num(); i++)
	{
		Players[i] = players[i];
		Players[i]->SetActorLocation(TeleportPoints[i]->GetActorLocation());
		Players[i]->SetActorRotation(TeleportPoints[i]->GetActorRotation());
		
		//TODO: 플레이어 앉은 포즈 시작
	}
}

void ULiarGameSystem::GameStart()
{
	//TODO: 모든 플레이어 시점을 전환.
	Multicast_ChooseLiar();
	Multicast_ShowKeyword();
}

void ULiarGameSystem::Multicast_ChooseLiar_Implementation()
{
	int randPlayer = FMath::RandRange(0, PlayerNum - 1);
	LiarPlayer = Players[randPlayer];
}

void ULiarGameSystem::Multicast_ShowKeyword_Implementation()
{
	for (ACharacter* player : Players)
	{
		player->SetActorRotation(FRotator(0, 0, -180));
	}
}
