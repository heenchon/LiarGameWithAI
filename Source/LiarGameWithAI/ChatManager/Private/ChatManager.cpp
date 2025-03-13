// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameWithAI/ChatManager/Public/ChatManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"


// Sets default values
AChatManager::AChatManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AChatManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChatManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// 서버에게 채팅 내용 보내기
void AChatManager::ServerRPC_SendChat_Implementation(const FString& userId, const FString& chat)
{
	// FUserChatInfo 생성 및 추가
	FUserChatInfo UserChatInfo;
	UserChatInfo.userId = userId;
	UserChatInfo.chat = chat;
	
	// 모든 클라이언트에게 보내자 (브로드캐스트)
	NetMulticast_SendChat(userId,chat);
	// AI에게도 채팅 내용을 전달하자
	SendChatToAI(userId,chat);
}

// 모든 클라이언트에게 채팅 내용 보내기
void AChatManager::NetMulticast_SendChat_Implementation(const FString& userId, const FString& chat)
{
	// 채팅 UI나 이런데서 입력받은 값을 넘겨주기?
	// 즉, 출력할 값을 넣어주면 된다
}

// AI에게 채팅 내용 전달
void AChatManager::SendChatToAI(const FString& userId, const FString& chat)
{
	// 채팅 내용을 Json형태로 변환해서 넘겨주어야 함
	
	// 서버에게 요청하는 객체 만들자
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	// 요청 URL - 서버가 알려줌
	httpRequest->SetURL(TEXT("http://192.168.10.78:8511/"));
	httpRequest->SetVerb(TEXT("POST"));
	httpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// 서버에게 보내고 싶은 데이터 값 (Json)
	FUserChatInfo info;
	// TODO: 여기 설정해주기 (연동필요)
	info.userId = userId;
	info.chat = chat;
	FString jsonString;
	FJsonObjectConverter::UStructToJsonObjectString(info, jsonString);
	httpRequest->SetContentAsString(jsonString);
	
	// 서버에게 요청을 한 후 응답이 오면 호출되는 함수 등록
	httpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
	{
		// 응답이 오면 실행됨
		// 성공
		if (bProcessedSuccessfully)
		{
			// 이름이 같아야한다 {} 안
			FString jsonString = FString::Printf(TEXT("{\"commentData\": %s}"), *Response->GetContentAsString()); 
			UE_LOG(LogTemp,Warning,TEXT("%s"),*jsonString);
		}
		// 실패
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("통신실패: %d"), Response->GetResponseCode());
		}
	});
	
	// 요청을 보내자
	httpRequest->ProcessRequest();
}
