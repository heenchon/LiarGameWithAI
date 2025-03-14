// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameState.h"

#include "LiarPlayerState.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "LiarGameWithAI/Chair.h"

void ALiarGameState::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AChair> It(GetWorld()); It; ++It)
	{
		Chairs.Add(*It);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%d"), Chairs.Num());

	// TODO: ChatManager 스폰

	// 게임의 전체를 비춰주는 카메라 찾기
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		CameraActor = Cast<ACameraActor>(PC->GetViewTarget());
	}
}

void ALiarGameState::LiarTest()
{
	// 임시데이터

	TArray<FPlayerInfo> Players;
	
	for (int32 i = 0; i < 6; i++)
	{
		FPlayerInfo info = 
		{
			.id = FString::Printf(TEXT("Test%d"), i),
			.liar = i < 1,
			.order = i + 1
		};
		
		Players.Add(info);
	}
	
	TestInfo.players = Players;
	TestInfo.common_keyword = TEXT("딸기");
	TestInfo.lair_keyword = TEXT("바나나");

	GameStart();
}

void ALiarGameState::GameStart()
{
	// TODO: ChatManager에게 게임의 정보를 요청하자

	// 받은 데이터를 가지고 매니저 initialize하자
	// 라운드 시작하자 -> 1번째부터 시작!

	// TODO: 임시 테스트.. 나중에 수정할 것
	TestInfo.players.Sort([](const FPlayerInfo& A, const FPlayerInfo& B)
	{
		return A.order < B.order;
	});
	
	for (int32 i = 0; i < TestInfo.players.Num(); i++)
	{
		PlayerList.Add(TestInfo.players[i]);
	}
	
	InitPlayer();
	InitKeyword(TestInfo.common_keyword, TestInfo.lair_keyword);
	ShowKeyword();
	Round();
}

void ALiarGameState::Round()
{
	++CurRound;
	CurrentOrder = 0;
	
	// order에 해당하는 Player를 향해 카메라를 돌린다
	UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerList[CurrentOrder].id);
}

void ALiarGameState::InitPlayer()
{
	if (PlayerList.IsEmpty())
	{
		UE_LOG(LogTemp,Warning,TEXT("플레이어 리스트 Empty"));
		return;
	}

	ACharacter* My = GetWorld()->GetFirstPlayerController()->GetCharacter();
	ALiarGameWithAICharacter* MyPlayer = Cast<ALiarGameWithAICharacter>(My);

	for (int i = 0; i < PlayerList.Num(); i++)
	{
		// playerList 아이디가 자신의 아이디와 같다면 PlayerController의 Pawn을 의자에 앉히자.

		FPlayerInfo info = PlayerList[i];

		if (MyPlayer->UserId == info.id)
		{
			IamLiar = info.liar;
			MyPlayer->SetSitAnim();
			MyPlayer->SetActorTransform(Chairs[i]->SitPosition->GetComponentTransform());
			continue;
		}
	
		Chairs[i]->SpawnPlayer(info.id, info.order, info.liar);
	}
}

void ALiarGameState::InitKeyword(FString common, FString lair)
{
	CommonKeyword = common;
	LairKeyword = lair;
}

void ALiarGameState::ShowKeyword()
{
	// 나의 아이디를 가지고 PlayerList에서 정보를 찾는다
	// 내가 라이어이면 라이어키워드를 출력

	FString value = IamLiar? LairKeyword : CommonKeyword;

	FString Msg = FString::Printf(TEXT("내 키워드: %s"), *value);
	ScreenLog(Msg);
}

void ALiarGameState::CollectAnswers(int order)
{
	// APlayerController* localCon = GetWorld()->GetFirstPlayerController();
	// if (localCon && localCon->IsLocalController())
	// {
	// 	ALiarPlayerState* localState = Cast<ALiarPlayerState>(localCon->PlayerState);
	// 	if (localState->playerInfo.order == order)
	// 	{
	// 		ScreenLog("Type short explanation of the keyword");
	// 		InputAnswer();
	// 	}
	// 	else
	// 	{
	// 		ScreenLog("Waiting for Others Answer...");
	// 		WaitingOthersAnswer();
	// 	}
	// }
}

void ALiarGameState::InputAnswer()
{
	//TODO: ai에게 입력 검사 받기
}

void ALiarGameState::WaitingOthersAnswer()
{
	//TODO: 다른 플레이어가 연관어 말했는지 확인하기
}

void ALiarGameState::ScreenLog(const FString& string)
{
	FString Msg = FString::Printf(TEXT("%s"), *string);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Msg);
}