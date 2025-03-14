// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyManager.h"

#include "EngineUtils.h"
#include "ChatManager/Public/ChatManager.h"

// Sets default values
ALobbyManager::ALobbyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALobbyManager::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (auto Manager = Cast<AChatManager>(*It))
		{
			ChatManager = Manager;
		}
	}
	
	StartWidget = CreateWidget<UGamePlayerWidget>(GetWorld(), WidgetPlayFactory);
	UE_LOG(LogTemp, Warning, TEXT("ALobbyManager::BeginPlay"));
	if (StartWidget)
	{
		StartWidget->AddToPlayerScreen();
	}
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->bShowMouseCursor = true;
}

// Called every frame
void ALobbyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALobbyManager::EnterLobby()
{
	ChatManager->EnterLobby();
}

void ALobbyManager::EnterLobbyCompleted(const FLobbyResponse& LobbyData)
{
	// 플레이어들을 받아서
	// 필드에 소환
}

