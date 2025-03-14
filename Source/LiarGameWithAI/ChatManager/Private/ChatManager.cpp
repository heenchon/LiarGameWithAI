// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameWithAI/ChatManager/Public/ChatManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "LiarGameWithAI/ChatManager/Public/ChatPanelUI.h"
#include "LiarGameWithAI/ChatManager/Public/ChatPlayerController.h"


class AChatPlayerController;
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

void AChatManager::LobbyCheck_Implementation()
{
}

void AChatManager::SendKeywords_Implementation(const FString& NormalKeyword,
                                               const FString& LiarKeyword)
{
	// 서버에게 요청하는 객체
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/docs#/default/set_keyword_keyword_post"));
	httpRequest->SetVerb(TEXT("POST"));
	httpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// JSON 객체 생성
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("normal_keyword"), NormalKeyword);
	JsonObject->SetStringField(TEXT("liar_keyword"), LiarKeyword);

	// JSON 문자열 변환
	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	httpRequest->SetContentAsString(RequestBody);  // JSON 데이터 설정
	
	// 서버에게 요청을 한 후 응답이 오면 호출되는 함수 등록
	httpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
	{
		// 응답이 오면 실행됨
		// 성공
		if (bProcessedSuccessfully)
		{
			// 응답 출력
		    FString ResponseContent = Response->GetContentAsString();
		    UE_LOG(LogTemp, Log, TEXT("응답: %s"), *ResponseContent);
			
			FKeywordResponse KeywordData;
			FJsonObjectConverter::JsonObjectStringToUStruct(ResponseContent, &KeywordData, 0, 0);
		}
		// 실패
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("통신실패: %d"), Response->GetResponseCode());
			return;
		}
	});
	
	// 요청을 보내자
	httpRequest->ProcessRequest();
}

void AChatManager::EnterLobby_Implementation()
{
	// 서버에게 요청하는 객체
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/docs#/default/make_room_lobby_post"));
	httpRequest->SetVerb(TEXT("POST"));
	httpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// 서버에게 요청을 한 후 응답이 오면 호출되는 함수 등록
	httpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
	{
		// 응답이 오면 실행됨
		// 성공
		if (bProcessedSuccessfully)
		{
			FString ResponseContent = Response->GetContentAsString();
			UE_LOG(LogTemp, Log, TEXT("응답: %s"), *ResponseContent);
			FLobbyResponse LobbyData;
			FJsonObjectConverter::JsonObjectStringToUStruct(ResponseContent, &LobbyData, 0, 0);		
		}
		// 실패
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("통신실패: %d"), Response->GetResponseCode());
			return;
		}
	});
	
	// 요청을 보내자
	httpRequest->ProcessRequest();
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
	// 현재 클라이언트의 로컬 플레이어 컨트롤러를 가져오고
	APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();
	if (LocalPlayerController && LocalPlayerController->IsLocalPlayerController())
	{
		// PlayerController에서 UI를 가져오자
		AChatPlayerController* ChatPlayerController = Cast<AChatPlayerController>(LocalPlayerController);
		if (ChatPlayerController && ChatPlayerController->chatPanel)
		{
			// UI 업데이트하자
			ChatPlayerController->chatPanel->UpdateChat(FText::FromString(userId), FText::FromString(chat));
		}
	}
}

// AI에게 채팅 내용 전달
void AChatManager::SendChatToAI(const FString& userId, const FString& chat)
{
	// 채팅 내용을 Json형태로 변환해서 넘겨주어야 함
	
	// 서버에게 요청하는 객체 만들자
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	// 요청 URL - 서버가 알려줌
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/docs"));
	// TODO: 물어봐야함
	httpRequest->SetVerb(TEXT("POST"));
	httpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// 서버에게 보내고 싶은 데이터 값 (Json)
	FUserChatInfo info;
	// TODO: 여기 설정해주기 (연동필요)
	info.userId = userId;
	info.chat = chat;
	FString jsonString;
	FJsonObjectConverter::UStructToJsonObjectString(info, jsonString);
	// POST요청일때임 / GET
	httpRequest->SetContentAsString(jsonString);
	
	// 서버에게 요청을 한 후 응답이 오면 호출되는 함수 등록
	httpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
	{
		// 응답이 오면 실행됨
		// 성공
		if (bProcessedSuccessfully)
		{
			// 변수를 받고 (AI차례라면?)
			// AI한테 받은값 출력
			// Response (Json)이걸 스트럭트로 바꿔
		}
		// 실패
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("통신실패: %d"), Response->GetResponseCode());
			return;
		}
	});
	
	// 요청을 보내자
	httpRequest->ProcessRequest();
}
