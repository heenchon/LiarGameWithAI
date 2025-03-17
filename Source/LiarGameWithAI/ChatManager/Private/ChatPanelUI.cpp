// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameWithAI/ChatManager/Public/ChatPanelUI.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "LiarGameWithAI/LiarGameWithAICharacter.h"
#include "LiarGameWithAI/ChatManager/Public/ChatManager.h"
#include "LiarGameWithAI/ChatManager/Public/ChatMessageUI.h"
#include "LiarGameWithAI/Game/LiarPlayerState.h"
#include "LiarGameWithAI/GamePlayerState/GamePlayerState.h"


void UChatPanelUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (ChatInputBox)
	{
		ChatInputBox->OnTextCommitted.AddDynamic(this, &UChatPanelUI::OnTextCommitted);
	}

	ChatManager = Cast<AChatManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AChatManager::StaticClass()));

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
			ALiarPlayerState* GamePlayerState = Cast<ALiarPlayerState>(PlayerController->PlayerState);
			
			if (GamePlayerState)
			{
				FString PlayerName = GamePlayerState->GetPlayerName();
				GamePlayerState->ServerRPC_SendChat(PlayerName, InText.ToString());
				// 텍스트창 비우기
				ChatInputBox->SetText(FText::GetEmpty());
				/*ACharacter* My = GetWorld()->GetFirstPlayerController()->GetCharacter();
				ALiarGameWithAICharacter* MyPlayer = Cast<ALiarGameWithAICharacter>(My);
				if (MyPlayer)
				{
					FString PlayerName = MyPlayer->UserId;
					GamePlayerState->ServerRPC_SendChat(PlayerName, InText.ToString());
					// 텍스트창 비우기
					ChatInputBox->SetText(FText::GetEmpty());
				}*/
			}
		}
	}
}
