// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameModeBase.h"

#include "LiarGameState.h"
#include "LiarGameSystem.h"
#include "Engine/TextureRenderTarget.h"
#include "GameFramework/Character.h"

ALiarGameModeBase::ALiarGameModeBase()
{
	GameStateClass = ALiarGameState::StaticClass();
}

void ALiarGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	ALiarGameState* MyGameState = GetGameState<ALiarGameState>();
	if (MyGameState && NewPlayer)
	{
		MyGameState->PlayerList.Add(NewPlayer);
	}
}


