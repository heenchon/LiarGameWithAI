// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameWithAI/ChatManager/Public/StartManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"



// Sets default values
AStartManager::AStartManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStartManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStartManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FPlayerInfoArray AStartManager::SendGameStartInfo()
{
	// 서버에게 요청하는 객체 만들자
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	// 요청 URL - 서버가 알려줌
	httpRequest->SetURL(TEXT("http://192.168.10.78:8511/"));
	// TODO: 물어봐야함
	httpRequest->SetVerb(TEXT("POST"));
	httpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// 서버에게 요청을 한 후 응답이 오면 호출되는 함수 등록
	httpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
	{
		// 응답이 오면 실행됨
		// 성공
		if (bProcessedSuccessfully)
		{
			// Json을 struct로 바꿔주자
			// 서버에게 보내고 싶은 데이터 값 (Json)
			// TODO: 중괄호 안 값이 같아야함
			FString jsonString = FString::Printf(TEXT("{\"commentData\": %s}"), *Response->GetContentAsString()); 
			FJsonObjectConverter::JsonObjectStringToUStruct(jsonString, &allGameStartData);
		}
		// 실패
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("통신실패: %d"), Response->GetResponseCode());
		}
	});
	
	// 요청을 보내자
	httpRequest->ProcessRequest();

	return allGameStartData;
}

