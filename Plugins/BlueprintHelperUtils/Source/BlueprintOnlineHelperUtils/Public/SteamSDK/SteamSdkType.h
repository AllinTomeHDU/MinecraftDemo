#pragma once

#include "CoreMinimal.h"
#include "ThirdParty/Steamworks/Steamv157/sdk/public/steam/steam_api.h"
#include "SteamSdkType.generated.h"


enum ESteamUniverse
{
	EUniverseInvalid = 0,
	EUniversePublic = 1,
	EUniverseBeta = 2,
	EUniverseInternal = 3,
	EUniverseDev = 4,
	// k_EUniverseRC = 5,				// no such universe anymore
	EUniverseMax
};

USTRUCT(BlueprintType)
struct BLUEPRINTONLINEHELPERUTILS_API FCSteamID
{
	GENERATED_BODY()

public:
	FCSteamID() {}
	FCSteamID(const uint64& InSteamID64)
	{
		ID.SteamID = InSteamID64;
	}
	FCSteamID(const CSteamID& InSteamID)
	{
		ID.SteamID = InSteamID.ConvertToUint64();
	}

	FORCEINLINE uint64 GetSteamID() const { return ID.SteamID; }

	FORCEINLINE uint32 GetAccountID() const { return ID.m_comp.m_unAccountID; }

	FORCEINLINE FString ToString() { return FString::Printf(TEXT("%llu"), ID.SteamID); }

private:
	union SteamID_t
	{
		struct SteamIDComponent_t
		{
#ifdef VALVE_BIG_ENDIAN
			ESteamUniverse		m_EUniverse : 8;			// universe this account belongs to
			unsigned int		m_EAccountType : 4;			// type of account - can't show as EAccountType, due to signed / unsigned difference
			unsigned int		m_unAccountInstance : 20;	// dynamic instance ID
			uint32				m_unAccountID : 32;			// unique account identifier
#else
			uint32				m_unAccountID : 32;			// unique account identifier
			unsigned int		m_unAccountInstance : 20;	// dynamic instance ID
			unsigned int		m_EAccountType : 4;			// type of account - can't show as EAccountType, due to signed / unsigned difference
			ESteamUniverse		m_EUniverse : 8;			// universe this account belongs to
#endif
		} m_comp;

		uint64 SteamID;
	} ID;
};

UENUM(BlueprintType)
enum class ESteamFriendOnlineState : uint8
{
	Offline,		// 离线
	Online,			// 在线
	Busy,			// 忙碌
	Away,			// 暂离
	Snooze,			// 睡眠
	LookingToTrade, // 寻求交易
	LookingToPlay,  // 寻求游戏
};

USTRUCT(BlueprintType)
struct BLUEPRINTONLINEHELPERUTILS_API FSteamFriendInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamSdkBPLibrary")
	FString SteamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamSdkBPLibrary")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamSdkBPLibrary")
	ESteamFriendOnlineState OnlineState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamSdkBPLibrary")
	FSlateBrush ProfileBrush;
};
