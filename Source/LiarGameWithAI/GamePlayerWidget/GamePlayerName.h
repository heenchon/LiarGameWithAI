// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GamePlayerName.generated.h"

/**
 * 
 */
UCLASS()
class LIARGAMEWITHAI_API UGamePlayerName : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PlayerName;
};
