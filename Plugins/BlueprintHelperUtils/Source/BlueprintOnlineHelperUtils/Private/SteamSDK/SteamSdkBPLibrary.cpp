#include "SteamSDK/SteamSdkBPLibrary.h"


FCSteamID USteamSdkBPLibrary::CreateFCSteamID(const FString& InSteamID)
{
	return static_cast<uint64>(FCString::Strtoui64(*InSteamID, nullptr, 10));
}

FString USteamSdkBPLibrary::GetSteamID()
{
	return SteamUser() ? FString::Printf(TEXT("%llu"), SteamUser()->GetSteamID().ConvertToUint64()) : TEXT("");
}

FString USteamSdkBPLibrary::GetAccountID()
{
	return SteamUser() ? FString::Printf(TEXT("%u"), SteamUser()->GetSteamID().GetAccountID()) : TEXT("");
}

FString USteamSdkBPLibrary::GetPersonaName()
{
	return SteamFriends() ? UTF8_TO_TCHAR(SteamFriends()->GetPersonaName()) : TEXT("");
}

FString USteamSdkBPLibrary::GetIPCountry()
{
	return SteamUtils() ? UTF8_TO_TCHAR(SteamUtils()->GetIPCountry()) : TEXT("");
}

bool USteamSdkBPLibrary::GetAvatarBrush(FSlateBrush& Brush)
{
	if (!SteamUser()) return false;

	SteamUser()->GetSteamID();
	return GetAvatarBrush(SteamUser()->GetSteamID(), Brush);
}

bool USteamSdkBPLibrary::GetTargetAvatarBrush(const FString& InSteamID, FSlateBrush& Brush)
{
	CSteamID SteamID(static_cast<uint64>(FCString::Strtoui64(*InSteamID, nullptr, 10)));
	return GetAvatarBrush(SteamID, Brush);
}

int32 USteamSdkBPLibrary::GetFriendCount()
{
	if (!SteamFriends()) return -1;

	return SteamFriends()->GetFriendCount(k_EFriendFlagImmediate);
}

void USteamSdkBPLibrary::GetFriendsInfo(TArray<FSteamFriendInfo>& Friends)
{
	Friends.Empty();
	int FriendCount = GetFriendCount();
	for (int i = 0; i < FriendCount; ++i)
	{
		FSteamFriendInfo Info;
		CSteamID FriendID = SteamFriends()->GetFriendByIndex(i, k_EFriendFlagImmediate);
		Info.SteamID = FString::Printf(TEXT("%llu"), FriendID.ConvertToUint64());
		Info.Name = UTF8_TO_TCHAR(SteamFriends()->GetFriendPersonaName(FriendID));
		Info.OnlineState = static_cast<ESteamFriendOnlineState>(SteamFriends()->GetFriendPersonaState(FriendID));
		GetAvatarBrush(FriendID, Info.ProfileBrush);
		Friends.Add(Info);
	}
}

void USteamSdkBPLibrary::GetRecentlyFriendsInfo(TArray<FSteamFriendInfo>& Friends)
{
	Friends.Empty();
	int PlayerCount = SteamFriends()->GetCoplayFriendCount();
	for (int i = 0; i < PlayerCount; ++i)
	{
		FSteamFriendInfo Info;
		CSteamID PlayerID = SteamFriends()->GetCoplayFriend(i);
		Info.SteamID = FString::Printf(TEXT("%llu"), PlayerID.ConvertToUint64());
		if (GetSteamID() == Info.SteamID) continue;	// 跳过自己
		Info.Name = UTF8_TO_TCHAR(SteamFriends()->GetFriendPersonaName(PlayerID));
		Info.OnlineState = static_cast<ESteamFriendOnlineState>(SteamFriends()->GetFriendPersonaState(PlayerID));
		GetAvatarBrush(PlayerID, Info.ProfileBrush);
		Friends.Add(Info);
	}
}

bool USteamSdkBPLibrary::GetAvatarBrush(const FCSteamID& SteamID, FSlateBrush& Brush, FVector2D ImageSize)
{
	if (!SteamFriends() || !SteamUtils()) return false;

	int AvatarHandle = SteamFriends()->GetLargeFriendAvatar(SteamID.GetSteamID());
	uint32 Width = 0, Height = 0;
	if (SteamUtils()->GetImageSize(AvatarHandle, &Width, &Height))
	{
		TArray<uint8> RGBA;
		RGBA.SetNum(Width * Height * 4);
		SteamUtils()->GetImageRGBA(AvatarHandle, RGBA.GetData(), RGBA.Num());
		UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);
		if (!Texture) return false;

		void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(TextureData, RGBA.GetData(), RGBA.Num());
		Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

		Texture->UpdateResource();
		Brush.SetResourceObject(Texture);
		Brush.SetImageSize(ImageSize);
		return true;
	}
	return false;
}
