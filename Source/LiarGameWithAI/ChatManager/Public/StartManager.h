// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StartManager.generated.h"

USTRUCT()
struct FGameStartInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	FString id;
	UPROPERTY(VisibleAnywhere)
	bool liar;
	UPROPERTY(VisibleAnywhere)
	int32 order;
};

USTRUCT()
struct FGameStartInfoArray
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	TArray<FGameStartInfo> startData;
};


UCLASS()
class LIARGAMEWITHAI_API AStartManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStartManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Json 정보 전달
	FGameStartInfoArray SendGameStartInfo(); //인자가 array?

	UPROPERTY(visibleAnywhere)
	FGameStartInfoArray allGameStartData;
};
