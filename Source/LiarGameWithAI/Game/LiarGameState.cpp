// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarGameState.h"

#include "LiarPlayerState.h"
#include "EngineUtils.h"
#include "LiarGameWithAI/Chair.h"
#include "Net/UnrealNetwork.h"

void ALiarGameState::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AChair> It(GetWorld()); It; ++It)
	{
		Chairs.Add(*It);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%d"), Chairs.Num());

	//TODO: ChatManager 스폰
}

void ALiarGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// DOREPLIFETIME(ALiarGameState, LiarPlayer);
	// DOREPLIFETIME(ALiarGameState, PlayerList);

	DOREPLIFETIME(ALiarGameState,PlayerStates);
}

void ALiarGameState::LiarTest()
{
	if (HasAuthority())
	{
		//ScreenLog(TEXT("Game Start!"));
		
		Multicast_GameStart();
		Multicast_Round();
		CurRound++;
		//Multicast_Round();
		//CurRound++;
	}
}

void ALiarGameState::Multicast_GameStart_Implementation()
{
	// 임시데이터

	for (int32 i = 0; i < 6; i++)
	{
		FPlayerInfo info = 
		{
			.id = FString::Printf(TEXT("Test%d"), i),
			.liar = i < 1 ? true : false,
			.order = i + 1
		};
		
		PlayerList.Add(info);
	}
	
	SortPlayer();
	InitPlayer();
	InitKeyword();
	ShowKeyword();
}

void ALiarGameState::Multicast_Round_Implementation()
{
	for (int curOrder = 1; curOrder < PlayerStates.Num() + 1; curOrder++)
	{
		CollectAnswers(curOrder);
	}
}

void ALiarGameState::SortPlayer()
{
	//UE_LOG(LogTemp,Warning,TEXT("플레이어 정렬"));
	
	// PlayerList.Sort([](const FPlayerInfo& A, const FPlayerInfo& B)
	// {
	// 	return A.order < B.order;
	// });
	
	PlayerStates.Sort([](const APlayerState& A, const APlayerState& B)
	{
		return A.GetPlayerId() < B.GetPlayerId();
	});
}

void ALiarGameState::InitPlayer()
{
	if (PlayerList.IsEmpty())
	{
		UE_LOG(LogTemp,Warning,TEXT("플레이어 리스트 Empty"));
		return;
	}

	if (PlayerStates.IsEmpty())
	{
		UE_LOG(LogTemp,Error,TEXT("플레이어 스테이트 Empty"));
	}
	
	//TODO: 플레이어 Json 정보 받아서 정렬.
	for (int i = 0; i < PlayerStates.Num(); i++)
	{
		PlayerStates[i]->playerInfo = PlayerList[i];
	}

	for (int i = 0; i < PlayerStates.Num(); i++)
	{
		PlayerStates[i]->GetPawn()->SetActorLocation(Chairs[i]->SitPosition->GetComponentLocation());
		PlayerStates[i]->GetPawn()->SetActorRotation(Chairs[i]->SitPosition->GetComponentRotation());
	}

	// for (int i = 0; i < PlayerList.Num(); i++)
	// {
	// 	// TODO: playerList 아이디가 자신의 아이디와 같다면 PlayerController의 Pawn을 의자에 앉히자.
	// 	
	// 	FPlayerInfo info = PlayerList[i];
	//
	// 	Chairs[i]->SpawnPlayer(info.id, info.order, info.liar);
	// }
}

void ALiarGameState::InitKeyword()
{
	//TODO: json의 키워드 정보 받아오기
	
	for (int i = 0; i < PlayerStates.Num(); i++)
	{
		if (PlayerStates[i]->playerInfo.liar)
		{
			PlayerStates[i]->Keyword = liar_keyword;
		}
		else
		{
			PlayerStates[i]->Keyword = common_keyword;
		}
	}
}

void ALiarGameState::ShowKeyword()
{
	APlayerController* localCon = GetWorld()->GetFirstPlayerController();
	if (localCon && localCon->IsLocalController())
	{
		ALiarPlayerState* localState = Cast<ALiarPlayerState>(localCon->PlayerState);
		if (localState)
		{
			FString Msg = FString::Printf(TEXT("내 키워드: %s"), *localState->Keyword);
			ScreenLog(Msg);
		}
	}
}

void ALiarGameState::CollectAnswers(int order)
{
	APlayerController* localCon = GetWorld()->GetFirstPlayerController();
	if (localCon && localCon->IsLocalController())
	{
		ALiarPlayerState* localState = Cast<ALiarPlayerState>(localCon->PlayerState);
		if (localState->playerInfo.order == order)
		{
			ScreenLog("Type short explanation of the keyword");
			InputAnswer();
		}
		else
		{
			ScreenLog("Waiting for Others Answer...");
			WaitingOthersAnswer();
		}
	}
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