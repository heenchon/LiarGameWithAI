// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyManager.h"

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

