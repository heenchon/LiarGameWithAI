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
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* RoadingBar;
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* MainPanel;
	
	float CurrentTime = 0.0f;

	UPROPERTY()
	class ALobbyManager* LobbyManager;

	UFUNCTION()
	void OnPlayerStartClicked();

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry & MyGeometry, float InDeltaTime) override;

	void GetLoadPlayer();
};
