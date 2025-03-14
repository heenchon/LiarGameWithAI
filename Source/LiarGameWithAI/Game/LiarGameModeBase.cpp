// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameModeBase.h"

#include "LiarGameState.h"
#include "LiarPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "LiarGameWithAI/LairGameInstance.h"

ALiarGameModeBase::ALiarGameModeBase()
{
	GameStateClass = ALiarGameState::StaticClass();
	PlayerStateClass = ALiarPlayerState::StaticClass();
}

void ALiarGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//
	// if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("GameMap"))
	// {
	// 	ALiarGameState* MyGameState = GetGameState<ALiarGameState>();
	// 	if (MyGameState)
	// 	{
	// 		ULairGameInstance* LairGameInstance = Cast<ULairGameInstance>(GetGameInstance());
	// 		if (LairGameInstance)
	// 		{
	// 			MyGameState->Initialize(LairGameInstance->GameInfo);
	// 		}
	// 	}
	// }
}

void ALiarGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	ALiarGameState* MyGameState = GetGameState<ALiarGameState>();
	// if (MyGameState && NewPlayer && NewPlayer->PlayerState)
	// {
	// 	ALiarPlayerState* LiarPS = Cast<ALiarPlayerState>(NewPlayer->PlayerState);
	// 	if(LiarPS)
	// 	{
	// 		MyGameState->PlayerList.Add(LiarPS);
	// 	}
	// }
}


