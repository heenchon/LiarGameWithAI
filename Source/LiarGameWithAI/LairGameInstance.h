// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LiarGameInfo.h"
#include "Engine/GameInstance.h"
#include "LairGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LIARGAMEWITHAI_API ULairGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UPROPERTY(EditAnywhere)
	FGameInfo GameInfo;
};
