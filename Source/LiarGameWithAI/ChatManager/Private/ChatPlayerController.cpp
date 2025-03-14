// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameWithAI/ChatManager/Public/ChatPlayerController.h"

#include "Components/ScrollBox.h"
#include "LiarGameWithAI/ChatManager/Public/ChatPanelUI.h"

void AChatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		chatPanel = CreateWidget<UChatPanelUI>(this, UChatPanelUIClass);
		chatPanel->SetOwningPlayer(this);
	}
		
}

void AChatPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Enter))
	{
		if (chatPanel)
		{
			chatPanel->ChatScrollBox->SetKeyboardFocus();
		}
	}
}
