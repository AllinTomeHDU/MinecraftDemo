// Copyright: Jichao Luo


#include "Room/UI/MultiplayerRoomWidgetBase.h"
#include "Core/MultiplayerSessionsSubsystem.h"


void UMultiplayerRoomWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	MultiplayerSessionsSubsystem = GetGameInstance()->GetSubsystem<UMultiplayerSessionsSubsystem>();
	if (IsValid(MultiplayerSessionsSubsystem))
	{
		MultiplayerSessionsSubsystem->OnMultiplayerSessionDestroyDelegate.AddUObject(this, &ThisClass::OnDestroySessionComplete);
		MultiplayerSessionsSubsystem->OnMultiplayerSessionStartDelegate.AddUObject(this, &ThisClass::OnStartSessionComplete);
		MultiplayerSessionsSubsystem->OnMultiplayerSessionEndDelegate.AddUObject(this, &ThisClass::OnEndSessionComplete);
		MultiplayerSessionsSubsystem->OnMultiplayerSessionUpdateDelegate.AddUObject(this, &ThisClass::OnUpdateSessionComplete);
	}

	GetWorld()->GetTimerManager().SetTimer(
		UpdateRoomHandle,
		this,
		&ThisClass::UpdateRoomWidget,
		UpdateRoomInterval,
		true
	);
}

void UMultiplayerRoomWidgetBase::UpdateRoomWidget()
{
	OnUpdateRoomWidget();
}

void UMultiplayerRoomWidgetBase::OnDestroySessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success to Destroy Session in Widget Base"));
		OnDestroySession();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Destroy Session in Widget Base"));
	}
}

void UMultiplayerRoomWidgetBase::OnStartSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success to Start Session in Widget Base"));
		OnStartSession();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Start Session in Widget Base"));
	}
}

void UMultiplayerRoomWidgetBase::OnEndSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success to End Session in Widget Base"));
		OnEndSession();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to End Session in Widget Base"));
	}
}

void UMultiplayerRoomWidgetBase::OnUpdateSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success to Update Session in Widget Base"));
		OnUpdateSession();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Update Session in Widget Base"));
	}
}
