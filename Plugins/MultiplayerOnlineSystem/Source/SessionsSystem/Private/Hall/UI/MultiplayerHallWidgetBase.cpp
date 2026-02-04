// Copyright: Jichao Luo


#include "Hall/UI/MultiplayerHallWidgetBase.h"
#include "Core/MultiplayerSessionsSubsystem.h"
#include "TimerManager.h"


void UMultiplayerHallWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	MultiplayerSessionsSubsystem = GetGameInstance()->GetSubsystem<UMultiplayerSessionsSubsystem>();
	if (IsValid(MultiplayerSessionsSubsystem))
	{
		MultiplayerSessionsSubsystem->OnMultiplayerSessionCreateDelegate.AddUObject(this, &ThisClass::OnCreateSessionComplete);
		MultiplayerSessionsSubsystem->OnMultiplayerSessionsFindDelegate.AddUObject(this, &ThisClass::OnFindSessionsComplete);
		MultiplayerSessionsSubsystem->OnMultiplayerSessionJoinDelegate.AddUObject(this, &ThisClass::OnJoinSessionComplete);
	}

	GetWorld()->GetTimerManager().SetTimer(
		UpdateHallHandle,
		this,
		&ThisClass::UpdateHallWidget,
		UpdateHallInterval,
		true
	);
}

void UMultiplayerHallWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();

	GetWorld()->GetTimerManager().ClearTimer(UpdateHallHandle);
}

void UMultiplayerHallWidgetBase::UpdateHallWidget()
{
	MultiplayerSessionsSubsystem->MultiplayerSessionsFind();
	OnUpdateHallWidget();
}

void UMultiplayerHallWidgetBase::OnCreateSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Success to Create Session in Widget Base"));
		OnCreateSession();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Create Session in Widget Base"));
	}
}

void UMultiplayerHallWidgetBase::OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSeccessful)
{
	TMap<FString, FSessionsSearchResult> SearchResultsMap;
	if (bWasSeccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success to Find Sessions in Widget Base: %d"), SessionResults.Num());
		for (auto& Result : SessionResults)
		{
			FSessionsSearchResult BPResult;
			BPResult.SessionResult = Result;
			BPResult.SessionID = Result.GetSessionIdStr();
			BPResult.PingMs = Result.PingInMs;
			Result.Session.SessionSettings.Get(FName("RoomName"), BPResult.RoomName);
			Result.Session.SessionSettings.Get(FName("DifficultyLevel"), BPResult.DifficultyLevel);
			Result.Session.SessionSettings.Get(FName("IsPublic"), BPResult.bIsPublic);
			Result.Session.SessionSettings.Get(FName("Password"), BPResult.Password);

			Result.Session.SessionSettings.Get(FName("MaxPlayerNum"), BPResult.MaxPlayers);
			Result.Session.SessionSettings.Get(FName("CurrentPlayerNum"), BPResult.CurrentPlayers);

			FString MatchType;
			Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);
			if (MatchType == TEXT("AllinTome"))
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, 
					FString::Printf(TEXT("Matched AllinTome: %d/%d"), BPResult.CurrentPlayers, BPResult.MaxPlayers));
				SearchResultsMap.Add(Result.GetSessionIdStr(), BPResult);
			}
		}
	}
	OnFindSessions(SearchResultsMap);
}

void UMultiplayerHallWidgetBase::OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type ResultType)
{
	OnJoinSession(static_cast<ESessionsJoinResult>(ResultType));
}

