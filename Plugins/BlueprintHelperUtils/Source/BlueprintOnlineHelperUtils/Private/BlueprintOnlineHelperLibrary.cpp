#include "BlueprintOnlineHelperLibrary.h"
#include "OnlineSubsystem.h"
#include "SteamSDK/SteamSdkBPLibrary.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSessionSettings.h"


FString UBlueprintOnlineHelperLibrary::GetOnlinePlatform()
{
	return IOnlineSubsystem::Get()->GetSubsystemName().ToString();
}

FString UBlueprintOnlineHelperLibrary::GetOnlineUserID()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem) return TEXT("");

	IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
	if (!Identity.IsValid()) return TEXT("");

	TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0);
	if (!UserId.IsValid()) return TEXT("");

	return UserId->ToString();
}

FString UBlueprintOnlineHelperLibrary::GetOnlineUserName()
{
	FString Platform = GetOnlinePlatform();
	if (Platform == TEXT("STEAM"))
	{
		return USteamSdkBPLibrary::GetPersonaName();
	}
	return TEXT("");
}

bool UBlueprintOnlineHelperLibrary::GetOnlineUserAvatarBrush(FSlateBrush& Brush)
{
	FString Platform = GetOnlinePlatform();
	if (Platform == TEXT("STEAM"))
	{
		 return USteamSdkBPLibrary::GetAvatarBrush(Brush);
	}
	return false;
}

void UBlueprintOnlineHelperLibrary::GetOnlineFriendsList(TArray<FOnlineFriendInfo>& Friends)
{
	Friends.Empty();
	FString Platform = GetOnlinePlatform();
	if (Platform == TEXT("STEAM"))
	{
		TArray<FSteamFriendInfo> SteamFriends;
		USteamSdkBPLibrary::GetFriendsInfo(SteamFriends);
		for (const FSteamFriendInfo& SteamFriend : SteamFriends)
		{
			FOnlineFriendInfo FriendInfo;
			FriendInfo.Account = SteamFriend.SteamID;
			FriendInfo.Name = SteamFriend.Name;
			FriendInfo.OnlineState = ConvertSteamFriendOnlineState(SteamFriend.OnlineState);
			FriendInfo.ProfileBrush = SteamFriend.ProfileBrush;
			Friends.Add(FriendInfo);
		}
	}
}

void UBlueprintOnlineHelperLibrary::GetOnlineRecentlyFriendsList(TArray<FOnlineFriendInfo>& Friends)
{
	Friends.Empty();
	FString Platform = GetOnlinePlatform();
	if (Platform == TEXT("STEAM"))
	{
		TArray<FSteamFriendInfo> SteamPlayers;
		USteamSdkBPLibrary::GetRecentlyFriendsInfo(SteamPlayers);
		for (const FSteamFriendInfo& SteamPlayer : SteamPlayers)
		{
			FOnlineFriendInfo PlayerInfo;
			PlayerInfo.Account = SteamPlayer.SteamID;
			PlayerInfo.Name = SteamPlayer.Name;
			PlayerInfo.OnlineState = ConvertSteamFriendOnlineState(SteamPlayer.OnlineState);
			PlayerInfo.ProfileBrush = SteamPlayer.ProfileBrush;
			Friends.Add(PlayerInfo);
		}
	}
}

void UBlueprintOnlineHelperLibrary::GetOnineSessionPlayersList(TArray<FOnlinePlayerInfo>& Players)
{
	Players.Empty();
	IOnlineSessionPtr SessionInterface = IOnlineSubsystem::Get()->GetSessionInterface();
	IOnlineIdentityPtr IdentityInterface = IOnlineSubsystem::Get()->GetIdentityInterface();
	if (!SessionInterface.IsValid() || !IdentityInterface.IsValid()) return;

	FNamedOnlineSession* Session = SessionInterface->GetNamedSession(NAME_GameSession);
	if (!Session) return;

	FString Platform = GetOnlinePlatform();
	for (const FUniqueNetIdRef& PlayerId : Session->RegisteredPlayers)
	{
		FOnlinePlayerInfo Info;
		Info.Account = PlayerId->ToString();
		Info.Name = IdentityInterface->GetPlayerNickname(*PlayerId);
		if (Platform == TEXT("STEAM"))
		{
			USteamSdkBPLibrary::GetTargetAvatarBrush(Info.Account, Info.ProfileBrush);
		}
		Players.Add(Info);
	}
}

EFriendOnlineState UBlueprintOnlineHelperLibrary::ConvertSteamFriendOnlineState(ESteamFriendOnlineState InState)
{
	switch (InState)
	{
	case ESteamFriendOnlineState::Offline:
		return EFriendOnlineState::Offline;
	case ESteamFriendOnlineState::Online:
	case ESteamFriendOnlineState::LookingToTrade:
	case ESteamFriendOnlineState::LookingToPlay:
		return EFriendOnlineState::Online;
	case ESteamFriendOnlineState::Busy:
		return EFriendOnlineState::Busy;
	case ESteamFriendOnlineState::Away:
	case ESteamFriendOnlineState::Snooze:
		return EFriendOnlineState::Away;
	}
	return EFriendOnlineState::Offline;
}
