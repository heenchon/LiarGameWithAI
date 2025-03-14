// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameWithAI/ChatManager/Public/ChatManager.h"

#include "EngineUtils.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
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
	httpRequest->SetURL(TEXT("http://192.168.20.118:8088/is_sentence"));
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
			
			// JSON 파싱
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseContent);

			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				TArray<FString> Keys;
				JsonObject->Values.GetKeys(Keys);

				if (Keys.Num() == 2 && Keys.Contains("user_id") && Keys.Contains("word"))
				{
					// 문장으로 판별된 경우
					FValidSentenceResponse SentenceData;
					SentenceData.UserID = JsonObject->GetStringField("user_id");
					SentenceData.Word = JsonObject->GetStringField("word");

					UE_LOG(LogTemp, Log, TEXT("Valid Sentence - User: %s, Word: %s"),
						   *SentenceData.UserID, *SentenceData.Word);
				}
				else if (Keys.Num() == 1 && JsonObject->GetStringField(Keys[0]) != "")
				{
					// 특정 조건에서 생성된 연관 단어
					FGeneratedWordResponse GeneratedData;
					GeneratedData.UserID = Keys[0];
					GeneratedData.GeneratedWord = JsonObject->GetStringField(Keys[0]);

					UE_LOG(LogTemp, Log, TEXT("Generated Word - User: %s, Word: %s"),
						   *GeneratedData.UserID, *GeneratedData.GeneratedWord);
				}
				else if (Keys.Num() == 1 && JsonObject->GetBoolField(Keys[0]) == false)
				{
					// 문장이 아닌 경우
					FInvalidSentenceResponse InvalidData;
					InvalidData.UserID = Keys[0];
					InvalidData.IsValid = false;

					UE_LOG(LogTemp, Log, TEXT("Invalid Sentence - User: %s"), *InvalidData.UserID);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("알 수 없는 응답 형식"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("JSON 파싱 실패"));
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
						PlayerRole.Keyword = RoleObject->GetStringField(TEXT("keyword"));
						PlayerRole.Liar = RoleObject->GetBoolField(TEXT("liar"));

						RolesData.Roles.Add(UserID, PlayerRole);
					}
				}

				// 결과 출력
				for (const auto& Pair : RolesData.Roles)
				{
					UE_LOG(LogTemp, Log, TEXT("UserID: %s, Keyword: %s, Liar: %s"),
						   *Pair.Key, *Pair.Value.Keyword, Pair.Value.Liar ? TEXT("true") : TEXT("false"));
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
						PlayerState.Keyword = PlayerStateObject->GetStringField(TEXT("keyword"));
						PlayerState.bIsLiar = PlayerStateObject->GetBoolField(TEXT("liar"));

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
		// 응답 본문을 문자열로 가져옴
		FString ResponseContent = Response->GetContentAsString();
		UE_LOG(LogTemp, Log, TEXT("응답: %s"), *ResponseContent);

		// 응답이 "true" 또는 "false"인지 확인
		bool bGameStarted = ResponseContent.Equals(TEXT("true"), ESearchCase::IgnoreCase);
        
		if (bGameStarted)
		{
			UE_LOG(LogTemp, Log, TEXT("게임이 시작되었습니다."));
			// 게임 시작 로직 처리
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("게임이 시작되지 않았습니다."));
			// 게임 시작 전 로직 처리
		}
	});
}

// AI에게 채팅 내용 전달
void AChatManager::SendChatToAI(const FString& userId, const FString& chat)
{
	// 채팅 내용을 Json형태로 변환해서 넘겨주어야 함
	
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
