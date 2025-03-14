// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyManager.h"

#include "EngineUtils.h"
#include "LairGameInstance.h"
#include "LiarGameWithAICharacter.h"
#include "ChatManager/Public/ChatManager.h"
#include "Kismet/GameplayStatics.h"
#include "LiarGameWithAI/LiarGameInfo.h"

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

	StartButton = CreateWidget<UGameStartButton>(GetWorld(), StartButtonFactory);
	if (StartButton)
	{
		StartButton->AddToViewport();
	}
	
	StartWidget = CreateWidget<UGamePlayerWidget>(GetWorld(), WidgetPlayFactory);
	if (StartWidget)
	{
		StartWidget->AddToViewport();
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
	// loadingui를 가리자

	
	// 지급 받은 내아이디 캐싱
	MyUserId = LobbyData.user_id;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	StartWidget->RemoveFromParent();
	
	bIamHost = LobbyData.is_host;
	
	if (LobbyData.room.Num() > 1)
	{
		for (int32 i = 0; i < LobbyData.room.Num(); i++)
		{
			if (LobbyData.room[i] == MyUserId)
			{
				if (ALiarGameWithAICharacter* Player = Cast<ALiarGameWithAICharacter>( PlayerController->GetCharacter()))
				{
					Player->SetUserId(MyUserId, true);
				}
				continue;
			}
			// 플레이어 소환
			//LobbyData.Room[i];
			FRotator rot = FRotator(0,i*60,0);
			FRotator MiRot = rot + FRotator(0,180,0);
			FVector RotLocation = GetActorLocation() + rot.Vector()*500;
			ALiarGameWithAICharacter* Player = GetWorld()->SpawnActor<ALiarGameWithAICharacter>(
			CharacterFactory, RotLocation, MiRot);
			Player->SetActorRotation(MiRot);
			Player->SetUserId(LobbyData.room[i], false);
			Players.Add(LobbyData.room[i]);
		}
	}
	
	GetWorld()->
	GetTimerManager()
	.SetTimer(
		LobbyCheckTimerHandle,
		[this] ()
		{
			ChatManager->LobbyCheck();
		},
		0.25f,
		true
	);

	if (LobbyData.is_host)
	{
		ChatManager->SendKeywords(TEXT("고양이"), TEXT("호랑이"));
	}
	else
	{
		GetWorld()->
		GetTimerManager()
		.SetTimer(
			StartCheckTimerHandle,
			[this] ()
			{
				ChatManager->StartCheck();
			},
			0.25f,
			true
		);
	}
}

void ALobbyManager::LobbyCheckCompleted(const FLobbyResponse& LobbyData)
{
	if (LobbyData.room.Num() > 1)
	{
		for (int32 i = 0; i < LobbyData.room.Num(); i++)
		{
			if (Players.Find(LobbyData.room[i]))
			{
				continue;
			}
			// 플레이어 소환
			//LobbyData.Room[i];
			FRotator rot = FRotator(0,i*60,0);
			FRotator MiRot = rot + FRotator(0,180,0);
			FVector RotLocation = GetActorLocation() + rot.Vector()*500;
			ALiarGameWithAICharacter* Player = GetWorld()->SpawnActor<ALiarGameWithAICharacter>(
			CharacterFactory, RotLocation, MiRot);
			Player->SetActorRotation(MiRot);
			Player->SetUserId(LobbyData.room[i], false);
			Players.Add(LobbyData.room[i]);
		}
	}
}

void ALobbyManager::StartCheckCompleted(const FGameInfo& GameData)
{
	ULairGameInstance* GameInstance = Cast<ULairGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->GameInfo = GameData;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("GameMap"));
}

void ALobbyManager::StartGame()
{
}

void ALobbyManager::StartGameCompleted(const FGameInfo& GameData)
{
	ULairGameInstance* GameInstance = Cast<ULairGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->GameInfo = GameData;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("GameMap"));
}


