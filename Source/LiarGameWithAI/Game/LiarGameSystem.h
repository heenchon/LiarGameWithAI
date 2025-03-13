// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LiarGameSystem.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIARGAMEWITHAI_API ULiarGameSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	ULiarGameSystem();
	
	UPROPERTY(VisibleAnywhere)
	TArray<ACharacter*> Players;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> TeleportPoints;

private:
	int PlayerNum;

	UPROPERTY()
	ACharacter* LiarPlayer;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	void InitPlayers(TArray<ACharacter*> players);

private:
	void GameStart();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ChooseLiar();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ShowKeyword();
};
