// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChatManager.generated.h"

USTRUCT()
struct FUserChatInfo
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString userId;
	UPROPERTY()
	FString chat;
};

USTRUCT()
struct FUserChatInfoArray
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FUserChatInfo> chats;
};

UCLASS()
class LIARGAMEWITHAI_API AChatManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChatManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 서버에게 채팅 내용 보내기
	UFUNCTION(Server, Reliable)
	void ServerRPC_SendChat(const FString& userId, const FString& chat);
	
	// 모든 클라이언트에게 채팅 내용 보내기
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SendChat(const FString& userId, const FString& chat);

	// AI에게 채팅 내용 전달 (Json으로 바꾸자)
	void SendChatToAI(const FString& userId, const FString& chat);

};
