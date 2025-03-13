// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFifth.h"


// Sets default values
APlayerFifth::APlayerFifth()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerFifth::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerFifth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerFifth::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

