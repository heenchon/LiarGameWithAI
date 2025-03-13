// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class LIARGAMEWITHAI_API UGamePlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UButton* PlayerStart;

	UFUNCTION()
	void OnPlayerStartClicked();

	virtual void NativeConstruct() override;
};
