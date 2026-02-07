#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SteamSdkType.h"
#include "SteamSdkBPLibrary.generated.h"



UCLASS()
class BLUEPRINTONLINEHELPERUTILS_API USteamSdkBPLibrary : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "SteamSdkBPLibrary")
	static FCSteamID CreateFCSteamID(const FString& InSteamID);


	/*
	* 个人信息
	*/
	UFUNCTION(BlueprintPure, Category = "SteamSdkBPLibrary")
	static FString GetSteamID();

	UFUNCTION(BlueprintPure, Category = "SteamSdkBPLibrary")
	static FString GetAccountID();

	UFUNCTION(BlueprintPure, Category = "SteamSdkBPLibrary")
	static FString GetPersonaName();

	UFUNCTION(BlueprintPure, Category = "SteamSdkBPLibrary")
	static FString GetIPCountry();

	UFUNCTION(BlueprintPure, Category = "SteamSdkBPLibrary")
	static bool GetAvatarBrush(FSlateBrush& Brush);

	UFUNCTION(BlueprintPure, Category = "SteamSdkBPLibrary")
	static bool GetTargetAvatarBrush(const FString& InSteamID, FSlateBrush& Brush);

	/*
	* 好友信息
	*/
	UFUNCTION(BlueprintPure, Category = "SteamSdkBPLibrary")
	static int32 GetFriendCount();

	UFUNCTION(BlueprintPure, Category = "SteamSdkBPLibrary")
	static void GetFriendsInfo(TArray<FSteamFriendInfo>& Friends);

	/*
	* 最近一起游戏的玩家
	*/
	UFUNCTION(BlueprintPure, Category = "SteamSdkBPLibrary")
	static void GetRecentlyFriendsInfo(TArray<FSteamFriendInfo>& Friends);

protected:
	static bool GetAvatarBrush(const FCSteamID& SteamID, FSlateBrush& Brush, FVector2D ImageSize = FVector2D(64.f, 64.f));


};