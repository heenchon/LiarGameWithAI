// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarPlayerState.h"

#include "Net/UnrealNetwork.h"

void ALiarPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALiarPlayerState, playerInfo);
}

void ALiarPlayerState::BeginPlay()
{
	Super::BeginPlay();
}
