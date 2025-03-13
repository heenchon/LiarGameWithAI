// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LiarGameWithAICharacter.h"
#include "GameFramework/Actor.h"
#include "Chair.generated.h"

UCLASS()
class LIARGAMEWITHAI_API AChair : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChair();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* RootMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* SitPosition;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ALiarGameWithAICharacter> PlayerFactory;
	
};
