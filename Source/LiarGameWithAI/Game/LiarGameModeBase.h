// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LiarGameModeBase.generated.h"

class ALiarGameState;
class ULiarGameSystem;
/**
 * 
 */
UCLASS()
class LIARGAMEWITHAI_API ALiarGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	ALiarGameModeBase();

private:
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:

};
