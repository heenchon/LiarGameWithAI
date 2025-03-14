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
	FString UserID;
	UPROPERTY()
	TArray<FString> Room;
	UPROPERTY()
	FString Host;
	UPROPERTY()
	bool Start;
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
	FString Keyword;
	UPROPERTY(BlueprintReadOnly)
	bool Liar;
};

USTRUCT(BlueprintType)
struct FRolesResponse
{
	GENERATED_BODY()
public:	
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, FPlayerRole> Roles;
};


USTRUCT(BlueprintType)
struct FValidSentenceResponse
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString UserID;
	UPROPERTY(BlueprintReadOnly)
	FString Word;
};

USTRUCT(BlueprintType)
struct FGeneratedWordResponse
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString UserID;
	UPROPERTY(BlueprintReadOnly)
	FString GeneratedWord;
};

USTRUCT(BlueprintType)
struct FInvalidSentenceResponse
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString UserID;
	UPROPERTY(BlueprintReadOnly)
	bool IsValid;
};

USTRUCT(BlueprintType)
struct FAIgeneratedWord
{
	GENERATED_BODY()

	UPROPERTY()
	FString RelatedWord;
};

USTRUCT(BlueprintType)
struct FVotePlayer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FString> MostVotePlayer;
};

USTRUCT(BlueprintType)
struct FCheckAnswer
{
	GENERATED_BODY()

	UPROPERTY()
	bool Answer;
	UPROPERTY()
	bool Liar;
};

USTRUCT(BlueprintType)
struct FCheckAIAnswer
{
	GENERATED_BODY()

	UPROPERTY()
	bool Answer;
	UPROPERTY()
	bool Liar;
};

USTRUCT(BlueprintType)
struct FPlayersStates
{
	GENERATED_BODY()

	UPROPERTY()
	FString Keyword; // 유저의 키워드
	UPROPERTY()
	bool bIsLiar; // 유저가 라이어인지 여부
};

USTRUCT(BlueprintType)
struct FLobbyCheck
{
	GENERATED_BODY()

	UPROPERTY()
	FString UserId;
	UPROPERTY()
	TArray<FString> Room;
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
	

	// 서버에게 채팅 내용 보내기
	UFUNCTION(Server, Reliable)
	void ServerRPC_SendChat(const FString& userId, const FString& chat);
	
	// 모든 클라이언트에게 채팅 내용 보내기
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SendChat(const FString& userId, const FString& chat);

	// AI에게 채팅 내용 전달 (Json으로 바꾸자)
	void SendChatToAI(const FString& userId, const FString& chat);

};
