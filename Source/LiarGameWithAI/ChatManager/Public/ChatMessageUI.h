// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatMessageUI.generated.h"

/**
 * 
 */
UCLASS()
class LIARGAMEWITHAI_API UChatMessageUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChatText;

	UPROPERTY()
	FText PlayerName;
	UPROPERTY()
	FText Chat;
};
