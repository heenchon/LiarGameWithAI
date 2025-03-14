// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerWidget.h"

#include "DetailTreeNode.h"
#include "EngineUtils.h"
#include "MaterialHLSLTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "LiarGameWithAI/LobbyManager.h"

void UGamePlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 버튼 클릭 이벤트 바인딩
	if (PlayerStart)
	{
		PlayerStart->OnClicked.AddDynamic(this, &UGamePlayerWidget::OnPlayerStartClicked);
	}

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (auto Manager = Cast<ALobbyManager>(*It))
		{
			LobbyManager = Manager;
		}
	}
}

void UGamePlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	CurrentTime += InDeltaTime;
	if (CurrentTime > 2.0f)
	{
		CurrentTime = 0.0f;
	}
	float LoadingPercentage = CurrentTime / 2.0f;
	
	RoadingBar->SetPercent(LoadingPercentage);
}

void UGamePlayerWidget::GetLoadPlayer()
{
	
}

void UGamePlayerWidget::OnPlayerStartClicked()
{
	MainPanel->SetVisibility(ESlateVisibility::Visible);
	PlayerStart->SetVisibility(ESlateVisibility::Hidden);

	LobbyManager->EnterLobby();
	
	//GetLoadPlayer();
	
	// this->RemoveFromViewport();
}
