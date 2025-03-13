// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameWithAI/ChatManager/Public/ChatPanelUI.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"
#include "Components/EditableTextBox.h"


void UChatPanelUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (ChatInputBox)
	{
		ChatInputBox->OnTextCommitted.AddDynamic(this, &UChatPanelUI::OnTextCommitted);
	}

}

void UChatPanelUI::UpdateChat(FText PlayerName, FText Chat)
{
	
}

void UChatPanelUI::OnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
	
}
