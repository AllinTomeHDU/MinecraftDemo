// Copyright: Jichao Luo


#include "Room/MultiplayerRoomGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "Core/MultiplayerSessionsSubsystem.h"
#include "Room/MultiplayerRoomController.h"


void AMultiplayerRoomGameMode::BeginPlay()
{
	Super::BeginPlay();

	SessionsSubsystem = GetGameInstance()->GetSubsystem<UMultiplayerSessionsSubsystem>();
}

void AMultiplayerRoomGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 当有玩家加入和退出Session的时候，主动更新Session
	if (SessionsSubsystem)
	{
		SessionsSubsystem->MultiplayerSessionUpdate(SessionsSubsystem->GetSessionCreateParams());
	}
}

void AMultiplayerRoomGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (SessionsSubsystem)
	{
		SessionsSubsystem->MultiplayerSessionUpdate(SessionsSubsystem->GetSessionCreateParams());
	}
	if (auto PC = Cast<AMultiplayerRoomController>(Exiting))
	{
		PC->Logout();
	}
}
