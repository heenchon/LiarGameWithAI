// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameWithAI/ChatManager/Public/ChatPanelUI.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "GameFramework/PlayerState.h"
#include "LiarGameWithAI/ChatManager/Public/ChatManager.h"
#include "LiarGameWithAI/ChatManager/Public/ChatMessageUI.h"


void UChatPanelUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (ChatInputBox)
	{
		ChatInputBox->OnTextCommitted.AddDynamic(this, &UChatPanelUI::OnTextCommitted);
	}

	ChatManager = Cast<AChatManager>(GetWorld()->SpawnActor(AChatManager::StaticClass()));

}

void UChatPanelUI::UpdateChat(const FText& PlayerName, const FText& Chat)
{
	UChatMessageUI* chatMessage = CreateWidget<UChatMessageUI>(this, UChatUIClass);
	if (chatMessage && ChatScrollBox)
	{
		chatMessage->PlayerName = PlayerName;
		chatMessage->Chat = Chat;
		ChatScrollBox->AddChild(chatMessage);
		ChatScrollBox->ScrollToEnd();
	}
}

void UChatPanelUI::OnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter && ChatManager)
	{
		// 플레이어 이름과 채팅 내용을 서버에 전송
		// TODO: 가져와야할 값 (플레이어 이름, 채팅내용)
		APlayerController* PlayerController = GetOwningPlayer();
		if (PlayerController)
		{
			FString PlayerName = PlayerController->GetPlayerState<APlayerState>()->GetPlayerName();
			ChatManager->ServerRPC_SendChat(PlayerName, InText.ToString());
			ChatInputBox->SetText(FText::GetEmpty());
		}
	}
}
