// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSixth.h"


// Sets default values
APlayerSixth::APlayerSixth()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerSixth::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerSixth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerSixth::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

