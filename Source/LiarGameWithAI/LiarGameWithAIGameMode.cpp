// Copyright Epic Games, Inc. All Rights Reserved.

#include "LiarGameWithAIGameMode.h"
#include "LiarGameWithAICharacter.h"
#include "UObject/ConstructorHelpers.h"

ALiarGameWithAIGameMode::ALiarGameWithAIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
