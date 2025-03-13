// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerWidget.h"
#include "Components/Button.h"

void UGamePlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 버튼 클릭 이벤트 바인딩
	if (PlayerStart)
	{
		PlayerStart->OnClicked.AddDynamic(this, &UGamePlayerWidget::OnPlayerStartClicked);
	}
}


void UGamePlayerWidget::OnPlayerStartClicked()
{
	this->RemoveFromViewport();
}