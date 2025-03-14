// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameWithAI/ChatManager/Public/ChatManager.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/SPIRV-Headers/include/spirv/unified1/spirv.h>

#include "EngineUtils.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "LiarGameWithAI/LiarGameInfo.h"
#include "LiarGameWithAI/LobbyManager.h"
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

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (auto Manager = Cast<ALobbyManager>(*It))
		{
			LobbyManager = Manager;
		}
	}
}

// Called every frame
void AChatManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChatManager::ReceiveAIGenerateWord_Implementation()
{
	// 서버에게 요청하는 객체
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/generated_related_word"));
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

			FAIgeneratedWord AIwordData;
			FJsonObjectConverter::JsonObjectStringToUStruct(ResponseContent, &AIwordData, 0, 0);
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

void AChatManager::IsSentence_Implementation(const FString& UserId, const FString& Word)
{
	// 서버에게 요청하는 객체
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/keyword"));
	httpRequest->SetVerb(TEXT("POST"));
	httpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// JSON 객체 생성
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("user_id"), UserId);
	JsonObject->SetStringField(TEXT("word"), Word);

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
			
			FIsSentance SentanceData;
			FJsonObjectConverter::JsonObjectStringToUStruct(ResponseContent, &SentanceData, 0, 0);
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
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/lobby"));
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

			LobbyManager->EnterLobbyCompleted(LobbyData);
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

void AChatManager::LobbyCheck_Implementation()
{
	// 서버에게 요청하는 객체
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/lobby_check"));
	httpRequest->SetVerb(TEXT("GET"));
	httpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	
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
			
			FLobbyCheck LobbyData;
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

void AChatManager::SendKeywords_Implementation(const FString& NormalKeyword,
                                               const FString& LiarKeyword)
{
	// 서버에게 요청하는 객체
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/keyword"));
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


void AChatManager::RecieveKeywords_Implementation()
{
	// 서버에게 요청하는 객체
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/response_keyword"));
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
			
			// JSON 파싱
			// 구조체는 이렇게 해야한다고 함...
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseContent);

			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				FRolesResponse RolesData;
				TSharedPtr<FJsonObject> RolesObject = JsonObject->GetObjectField(TEXT("roles"));

				for (const auto& Entry : RolesObject->Values)
				{
					FString UserID = Entry.Key;
					TSharedPtr<FJsonObject> RoleObject = Entry.Value->AsObject();

					if (RoleObject.IsValid())
					{
						FPlayerRole PlayerRole;
						PlayerRole.keyword = RoleObject->GetStringField(TEXT("keyword"));
						PlayerRole.liar = RoleObject->GetBoolField(TEXT("liar"));

						RolesData.roles.Add(UserID, PlayerRole);
					}
				}

				// 결과 출력
				for (const auto& Pair : RolesData.roles)
				{
					UE_LOG(LogTemp, Log, TEXT("UserID: %s, Keyword: %s, Liar: %s"),
						   *Pair.Key, *Pair.Value.keyword, Pair.Value.liar ? TEXT("true") : TEXT("false"));
				}
			}
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


void AChatManager::VotePlayer_Implementation(const FString& UserId, const FString& ToId)
{
	// 서버에게 요청하는 객체
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/point"));
	httpRequest->SetVerb(TEXT("POST"));
	httpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// JSON 객체 생성
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("id"), UserId);
	JsonObject->SetStringField(TEXT("to_id"), ToId);

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
			
			FVotePlayer VoteData;
			FJsonObjectConverter::JsonObjectStringToUStruct(ResponseContent, &VoteData, 0, 0);
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

void AChatManager::CheckPlayerAnswer_Implementation(const FString& UserId, const FString& Answer)
{
	// 서버에게 요청하는 객체
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/check"));
	httpRequest->SetVerb(TEXT("POST"));
	httpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// JSON 객체 생성
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("user_id"), UserId);
	JsonObject->SetStringField(TEXT("answer"), Answer);

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
			
			FCheckAnswer CheckAnswerData;
			FJsonObjectConverter::JsonObjectStringToUStruct(ResponseContent, &CheckAnswerData, 0, 0);
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

void AChatManager::CheckAIAnswer_Implementation()
{
	// 서버에게 요청하는 객체
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/ai_answer"));
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
			FCheckAIAnswer AIAnswer;
			FJsonObjectConverter::JsonObjectStringToUStruct(ResponseContent, &AIAnswer, 0, 0);		
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

void AChatManager::GameState_Implementation()
{
	// 서버에게 요청하는 객체
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/game_state"));
	httpRequest->SetVerb(TEXT("GET"));
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
			// JSON 객체 생성
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseContent);

			// JSON 파싱
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				// 1. 방의 모든 유저 ID 리스트
				TArray<FString> RoomUsers;
				const TArray<TSharedPtr<FJsonValue>>* RoomArray;
				if (JsonObject->TryGetArrayField(TEXT("room"), RoomArray))
				{
					for (const TSharedPtr<FJsonValue>& UserID : *RoomArray)
					{
						RoomUsers.Add(UserID->AsString());
					}
				}

				// 2. 게임 시작 여부
				bool bGameStarted = JsonObject->GetBoolField(TEXT("start"));

				// 3. 남은 사이클 수
				int32 RemainingCycles = JsonObject->GetIntegerField(TEXT("cycle"));

				// 4. 입력된 단어 리스트
				TArray<FString> TotalWords;
				const TArray<TSharedPtr<FJsonValue>>* WordsArray;
				if (JsonObject->TryGetArrayField(TEXT("total_word"), WordsArray))
				{
					for (const TSharedPtr<FJsonValue>& Word : *WordsArray)
					{
						TotalWords.Add(Word->AsString());
					}
				}

				// 5. 각 유저의 상태 정보
				TMap<FString, FPlayersStates> Players;
				const TSharedPtr<FJsonObject>* PlayersObject;
				if (JsonObject->TryGetObjectField(TEXT("players"), PlayersObject))
				{
					for (const TPair<FString, TSharedPtr<FJsonValue>>& PlayerPair : (*PlayersObject)->Values)
					{
						FString UserID = PlayerPair.Key;
						TSharedPtr<FJsonObject> PlayerStateObject = PlayerPair.Value->AsObject();

						FPlayersStates PlayerState;
						PlayerState.keyword = PlayerStateObject->GetStringField(TEXT("keyword"));
						PlayerState.liar = PlayerStateObject->GetBoolField(TEXT("liar"));

						Players.Add(UserID, PlayerState);
					}
				}
			}
		}
	});
}


void AChatManager::DevClear_Implementation()
{
	// 서버에게 요청하는 객체
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/dev_clear"));
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
		}
		// 실패
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("통신실패: %d"), Response->GetResponseCode());
			return;
		}
	});
}

void AChatManager::StartCheck_Implementation()
{
	// 서버에게 요청하는 객체
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/start_check"));
	httpRequest->SetVerb(TEXT("GET"));
	httpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// 서버에게 요청을 한 후 응답이 오면 호출되는 함수 등록
	httpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
	{
		if (bProcessedSuccessfully)
		{
			FString ResponseContent = Response->GetContentAsString();
			UE_LOG(LogTemp, Log, TEXT("Start Check 응답: %s"), *ResponseContent);
			FGameInfo GameData;
			FJsonObjectConverter::JsonObjectStringToUStruct(ResponseContent, &GameData, 0, 0);

			if (LobbyManager && GameData.players.Num() > 0)
			{
				LobbyManager->StartCheckCompleted(GameData);
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("게임이 시작되지 않았습니다."));
			// 게임 시작 전 로직 처리
		}
	});

	// 요청을 보내자
	httpRequest->ProcessRequest();
}

void AChatManager::GameStart_Implementation()
{
	// 서버에게 요청하는 객체
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/game_start"));
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
			FGameInfo GameData;
			FJsonObjectConverter::JsonObjectStringToUStruct(ResponseContent, &GameData, 0, 0);

			if (LobbyManager)
			{
				LobbyManager->StartGameCompleted(GameData);
			}
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

/*
// AI에게 채팅 내용 전달
void AChatManager::SendChatToAI(const FString& userId, const FString& chat)
{
	// 서버에게 요청하는 객체 만들자
	FHttpRequestRef httpRequest = FHttpModule::Get().CreateRequest();
	// 요청 URL - 서버가 알려줌
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/"));
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
*/
