// Copyright: Jichao Luo


#include "Room/MultiplayerRoomHUD.h"
#include "Room/UI/MultiplayerRoomWidgetBase.h"
#include "Room/MultiplayerRoomSubsystem.h"


void AMultiplayerRoomHUD::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
		FString::Printf(TEXT("MultiplayerRoomHUD BeginPlay: %d"), HasAuthority()));

	if (auto RoomSubsystem = GetGameInstance()->GetSubsystem<UMultiplayerRoomSubsystem>())
	{
		if (RoomSubsystem->IsTransiotionPlaying())
		{
			RoomSubsystem->OnTransionWidgetCloseDelegate.AddUObject(this, &ThisClass::CreateRoomWidget);
		}
		else
		{
			CreateRoomWidget();
		}
	}
}

void AMultiplayerRoomHUD::CreateRoomWidget()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("CreateRoomWidget called"));
	checkf(RoomWidgetClass, TEXT("RoomWidgetClass is nullptr"));
	RoomWidget = CreateWidget<UMultiplayerRoomWidgetBase>(GetWorld(), RoomWidgetClass);
	if (IsValid(RoomWidget))
	{
		RoomWidget->AddToViewport();
	}
}
