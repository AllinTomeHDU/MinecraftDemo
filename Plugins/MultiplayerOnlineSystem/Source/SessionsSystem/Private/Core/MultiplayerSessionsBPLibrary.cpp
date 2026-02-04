// Copyright: Jichao Luo


#include "Core/MultiplayerSessionsBPLibrary.h"
#include "GameFramework/PlayerController.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineSessionInterface.h"


bool UMultiplayerSessionsBPLibrary::MultiplayerClientTravel(APlayerController* PlayerController, const FString& Address, ELevelTravelType TravelType)
{
	if (IsValid(PlayerController))
	{
		PlayerController->ClientTravel(Address, static_cast<ETravelType>(TravelType));
		return true;
	}
	return false;
}

bool UMultiplayerSessionsBPLibrary::MultiplayerServerTravel(UObject* WorldContextObject, const FString& Address, bool bAbsolute, bool bShouldSkipGameNotify)
{
	if (!IsValid(WorldContextObject)) return false;

	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (IsValid(World))
	{
		return World->ServerTravel(Address, bAbsolute, bShouldSkipGameNotify);
	}
	return false;
}

int32 UMultiplayerSessionsBPLibrary::GetSessionPlayerNumbers(FName SessionName)
{
	SessionName = SessionName == FName("PartySession") ? NAME_PartySession : NAME_GameSession;

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem) return 0;

	IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
	if (!SessionInterface.IsValid()) return 0;

	FNamedOnlineSession* CurrentSession = SessionInterface->GetNamedSession(SessionName);
	if (!CurrentSession) return 0;

	return CurrentSession->RegisteredPlayers.Num();
}

FString UMultiplayerSessionsBPLibrary::GetSessionID(FName SessionName)
{
	SessionName = SessionName == FName("PartySession") ? NAME_PartySession : NAME_GameSession;

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem) return TEXT("");

	IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
	if (!SessionInterface.IsValid()) return TEXT("");

	FNamedOnlineSession* CurrentSession = SessionInterface->GetNamedSession(SessionName);
	if (!CurrentSession) return TEXT("");

	return CurrentSession->GetSessionIdStr();
}

FSessionsCreateParams UMultiplayerSessionsBPLibrary::GetSessionCreateParams()
{
	FSessionsCreateParams Params;

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem) return Params;

	IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
	if (!SessionInterface.IsValid()) return Params;

	FNamedOnlineSession* NamedSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (!NamedSession) return Params;

	const FOnlineSessionSettings& Settings = NamedSession->SessionSettings;
	Settings.Get(FName("RoomName"), Params.RoomName);
	Settings.Get(FName("DifficultyLevel"), Params.DifficultyLevel);
	Settings.Get(FName("IsPublic"), Params.bIsPublic);
	Settings.Get(FName("Password"), Params.Password);
	Settings.Get(FName("MaxPlayerNum"), Params.MaxPlayers);
	return Params;
}

int32 UMultiplayerSessionsBPLibrary::GetSessionCurrentPlayerNumber()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem) return -1;

	IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
	if (!SessionInterface.IsValid()) return -1;

	FNamedOnlineSession* NamedSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (!NamedSession) return -1;

	return NamedSession->SessionSettings.NumPublicConnections - NamedSession->NumOpenPublicConnections;
}

