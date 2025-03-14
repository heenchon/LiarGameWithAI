// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStartButton.generated.h"

/**
 * 
 */
UCLASS()
class LIARGAMEWITHAI_API UGameStartButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* LastButton;
};
