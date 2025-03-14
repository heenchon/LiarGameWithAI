// Fill out your copyright notice in the Description page of Project Settings.


#include "LiarPlayerState.h"

#include "LiarGameWithAI/ChatManager/Public/ChatPlayerController.h"
#include "Net/UnrealNetwork.h"

class AChatPlayerController;

void ALiarPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALiarPlayerState, playerInfo);
}

void ALiarPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ALiarPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

// 서버에게 채팅 내용 보내기
void ALiarPlayerState::ServerRPC_SendChat_Implementation(const FString& userId, const FString& chat)
{
	// FUserChatInfo 생성 및 추가
	FUserChatInfo UserChatInfo;
	UserChatInfo.userId = userId;
	UserChatInfo.chat = chat;
	
	// 모든 클라이언트에게 보내자 (브로드캐스트)
	NetMulticast_SendChat(userId,chat);

	// AI한테도 채팅 내용을 보내자

}

// 모든 클라이언트에게 채팅 내용 보내기
void ALiarPlayerState::NetMulticast_SendChat_Implementation(const FString& userId, const FString& chat)
{
	// 모든 플레이어 컨트롤러에게 채팅 메시지 전달
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController)
		{
			AChatPlayerController* MyPlayerController = Cast<AChatPlayerController>(PlayerController);
			if (MyPlayerController && MyPlayerController->chatPanel)
			{
				MyPlayerController->chatPanel->UpdateChat(FText::FromString(userId), FText::FromString(chat));
			}
		}
	}
}