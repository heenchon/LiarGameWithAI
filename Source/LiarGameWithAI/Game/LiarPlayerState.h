// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LiarGameWithAI/LiarGameInfo.h"
#include "LiarPlayerState.generated.h"

/**
 * 
 */

UCLASS()
class LIARGAMEWITHAI_API ALiarPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, VisibleAnywhere, Category="Info")
	FPlayerInfo playerInfo;

	UPROPERTY(Replicated, VisibleAnywhere)
	FString Keyword;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	// 서버에게 채팅 내용 보내기
	UFUNCTION(Server, Reliable)
	void ServerRPC_SendChat(const FString& userId, const FString& chat);
	
	// 모든 클라이언트에게 채팅 내용 보내기
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SendChat(const FString& userId, const FString& chat);
};
