// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatPanelUI.h"
#include "GameFramework/PlayerController.h"
#include "ChatPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LIARGAMEWITHAI_API AChatPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> UChatPanelUIClass;
	UPROPERTY()
	UChatPanelUI* chatPanel;

	AChatManager* ChatManager;
	
};


