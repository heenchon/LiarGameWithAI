// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameModeBase.h"

#include "LiarGameState.h"
#include "LiarPlayerState.h"

ALiarGameModeBase::ALiarGameModeBase()
{
	GameStateClass = ALiarGameState::StaticClass();
	PlayerStateClass = ALiarPlayerState::StaticClass();
}

void ALiarGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	ALiarGameState* MyGameState = GetGameState<ALiarGameState>();
	if (MyGameState && NewPlayer && NewPlayer->PlayerState)
	{
		ALiarPlayerState* LiarPS = Cast<ALiarPlayerState>(NewPlayer->PlayerState);
		if(LiarPS)
		{
			MyGameState->PlayerList.Add(LiarPS);
		}
	}
}


