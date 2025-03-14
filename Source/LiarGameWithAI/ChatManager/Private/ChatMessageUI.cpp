// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameWithAI/ChatManager/Public/ChatMessageUI.h"

#include "Components/TextBlock.h"

void UChatMessageUI::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerNameText->SetText(PlayerName);
	ChatText->SetText(Chat);
}
