// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChatManager.generated.h"

USTRUCT(BlueprintType)
struct FUserChatInfo
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString userId;
	UPROPERTY()
	FString chat;
};

USTRUCT(BlueprintType)
struct FLobbyResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FString user_id;
	UPROPERTY()
	TArray<FString> room;
	UPROPERTY()
	bool is_host;
};

USTRUCT(BlueprintType)
struct FKeywordResponse
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString message;
};

USTRUCT(BlueprintType)
struct FPlayerRole
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	FString keyword;
	UPROPERTY(BlueprintReadOnly)
	bool liar;
};

USTRUCT(BlueprintType)
struct FRolesResponse
{
	GENERATED_BODY()
public:	
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, FPlayerRole> roles;
};


USTRUCT(BlueprintType)
struct FIsSentance
{
	GENERATED_BODY()

	UPROPERTY()
	FString user_id;
	UPROPERTY()
	FString word;
	UPROPERTY()
	bool success;
};

USTRUCT(BlueprintType)
struct FAIgeneratedWord
{
	GENERATED_BODY()

	UPROPERTY()
	FString related_word;
};

USTRUCT(BlueprintType)
struct FVotePlayer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FString> most_voted;
};

USTRUCT(BlueprintType)
struct FCheckAnswer
{
	GENERATED_BODY()

	UPROPERTY()
	bool answer;
	UPROPERTY()
	bool liar;
};

USTRUCT(BlueprintType)
struct FCheckAIAnswer
{
	GENERATED_BODY()

	UPROPERTY()
	bool answer;
	UPROPERTY()
	bool liar;
};

USTRUCT(BlueprintType)
struct FPlayersStates
{
	GENERATED_BODY()

	UPROPERTY()
	FString keyword; // 유저의 키워드
	UPROPERTY()
	bool liar; // 유저가 라이어인지 여부
};

USTRUCT(BlueprintType)
struct FLobbyCheck
{
	GENERATED_BODY()

	UPROPERTY()
	FString user_id;
	UPROPERTY()
	TArray<FString> room;
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

	UFUNCTION(Server, Reliable)
	void EnterLobby();
	UFUNCTION(Server, Reliable)
	void SendKeywords(const FString& NormalKeyword, const FString& LiarKeyword);
	UFUNCTION(Server, Reliable)
	void RecieveKeywords();
	UFUNCTION(Server, Reliable)
	void IsSentence(const FString& UserId, const FString& Word);
	UFUNCTION(Server, Reliable)
	void ReceiveAIGenerateWord();
	UFUNCTION(Server, Reliable)
	void VotePlayer(const FString& UserId, const FString& ToId);
	UFUNCTION(Server, Reliable)
	void CheckPlayerAnswer(const FString& UserId, const FString& Answer);
	UFUNCTION(Server, Reliable)
	void CheckAIAnswer();
	UFUNCTION(Server, Reliable)
	void GameState();
	
	UFUNCTION(Server, Reliable)
	void LobbyCheck();
	UFUNCTION(Server, Reliable)
	void DevClear();
	UFUNCTION(Server, Reliable)
	void StartCheck();

	UFUNCTION(Server, Reliable)
	void GameStart();

	/*// 서버에게 채팅 내용 보내기
	UFUNCTION(Server, Reliable)
	void ServerRPC_SendChat(const FString& userId, const FString& chat);
	
	// 모든 클라이언트에게 채팅 내용 보내기
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SendChat(const FString& userId, const FString& chat);*/

	// AI에게 채팅 내용 전달 (Json으로 바꾸자)
	void SendChatToAI(const FString& userId, const FString& chat);

	class ALobbyManager* LobbyManager;

};
