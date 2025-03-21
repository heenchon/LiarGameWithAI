// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GamePlayerWidget/GamePlayerWidget.h"
#include "LobbyManager.generated.h"

UCLASS()
class LIARGAMEWITHAI_API ALobbyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALobbyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UGamePlayerWidget> WidgetPlayFactory;

	UPROPERTY(VisibleAnywhere)
	class UGamePlayerWidget* StartWidget;
};
