#include "Chair.h"

AChair::AChair()
{
	PrimaryActorTick.bCanEverTick = false;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	SetRootComponent(RootMesh);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Props/SM_Chair.SM_Chair'"));

	if (tempMesh.Succeeded())
	{
		RootMesh->SetStaticMesh(tempMesh.Object);
	}

	SitPosition = CreateDefaultSubobject<USceneComponent>(TEXT("SitPosition"));
	SitPosition->SetupAttachment(RootMesh);
	SitPosition->SetRelativeLocation(FVector(20.f, 0.f, 60.f));
}

void AChair::BeginPlay()
{
	Super::BeginPlay();

	// Test...
}

void AChair::SpawnPlayer(FString userId, int32 order, bool isLair)
{
	UserId = userId;
	Order = order;
	IsLair = isLair;
	
	FActorSpawnParameters Params;
	ALiarGameWithAICharacter* Character = GetWorld()->SpawnActor<ALiarGameWithAICharacter>(PlayerFactory, SitPosition->GetComponentTransform(), Params);
	Character->SetUserId(userId, false);
	Character->SetSitAnim();
}

