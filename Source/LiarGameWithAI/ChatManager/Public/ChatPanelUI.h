// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatManager.h"
#include "Blueprint/UserWidget.h"
#include "ChatPanelUI.generated.h"

/**
 * 
 */
UCLASS()
class LIARGAMEWITHAI_API UChatPanelUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();

	//Widget 뷰포트에
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> UChatUIClass;
	
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ChatScrollBox;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ChatInputBox;

	void UpdateChat(const FText& PlayerName, const FText& Chat);

	UFUNCTION()
	void OnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod);

	AChatManager* ChatManager;
};
