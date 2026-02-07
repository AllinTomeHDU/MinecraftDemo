#pragma once

#include "CoreMinimal.h"
#include "BlueprintOnlineHelperType.generated.h"


UENUM(BlueprintType)
enum class EFriendOnlineState : uint8
{
	Offline,
	Online,
	Busy,
	Away
};

USTRUCT(BlueprintType)
struct BLUEPRINTONLINEHELPERUTILS_API FOnlineFriendInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamSdkBPLibrary")
	FString Account;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamSdkBPLibrary")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamSdkBPLibrary")
	EFriendOnlineState OnlineState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamSdkBPLibrary")
	FSlateBrush ProfileBrush;
};

UENUM(BlueprintType)
enum class EPlayerOnlineState : uint8
{
	Unprepared,
	Prepared,
	NetworkError
};

USTRUCT(BlueprintType)
struct BLUEPRINTONLINEHELPERUTILS_API FOnlinePlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamSdkBPLibrary")
	FString Account;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamSdkBPLibrary")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamSdkBPLibrary")
	EPlayerOnlineState State = EPlayerOnlineState::Unprepared;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamSdkBPLibrary")
	FSlateBrush ProfileBrush;
};

