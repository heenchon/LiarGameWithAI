// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LiarGameWithAI/LiarGameWithAICharacter.h"
#include "PlayerFifth.generated.h"

UCLASS()
class LIARGAMEWITHAI_API APlayerFifth : public ALiarGameWithAICharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerFifth();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
