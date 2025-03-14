// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyManager.h"

#include "EngineUtils.h"
#include "LiarGameWithAICharacter.h"
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
	// loadingui를 가리자

	
	// 지급 받은 내아이디 캐싱
	MyUserId = LobbyData.UserID;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	// FActorSpawnParameters SpawnParams;
	// SpawnParams.Owner = this;
	// SpawnParams.Instigator = GetInstigator();

	StartWidget->RemoveFromParent();
	
	if (ALiarGameWithAICharacter* Player = Cast<ALiarGameWithAICharacter>( PlayerController->GetCharacter()))
	{
		Player->SetUserId(MyUserId, true);
	}

	if (LobbyData.Room.Num() > 1)
	{
		for (int32 i = 0; i < LobbyData.Room.Num(); i++)
		{
			if (LobbyData.Room[i] == MyUserId)
			{
				continue;
			}
			// 플레이어 소환
			//LobbyData.Room[i];
			FRotator rot = FRotator(0,i*60,0);
			FVector RotLocation = GetActorLocation() + FVector(1,0,0) * rot.Vector()*500;
			ALiarGameWithAICharacter* Player = GetWorld()->SpawnActor<ALiarGameWithAICharacter>(
			CharacterFactory, RotLocation, rot);
			Player->SetActorRotation(rot);
		}
	}
	
	// 플레이어들을 받아서
	// 필드에 소환
}

