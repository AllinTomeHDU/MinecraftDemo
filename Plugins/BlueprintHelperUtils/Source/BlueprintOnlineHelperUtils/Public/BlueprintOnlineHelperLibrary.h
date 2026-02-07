#pragma once

#include "CoreMinimal.h"
#include "Definition/BlueprintOnlineHelperType.h"
#include "SteamSDK/SteamSdkType.h"
#include "BlueprintOnlineHelperLibrary.generated.h"


UCLASS()
class BLUEPRINTONLINEHELPERUTILS_API UBlueprintOnlineHelperLibrary : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "BlueprintOnlineHelperUtils")
	static FString GetOnlinePlatform();

	UFUNCTION(BlueprintPure, Category = "BlueprintOnlineHelperUtils")
	static FString GetOnlineUserID();

	UFUNCTION(BlueprintPure, Category = "BlueprintOnlineHelperUtils")
	static FString GetOnlineUserName();

	UFUNCTION(BlueprintPure, Category = "BlueprintOnlineHelperUtils")
	static bool GetOnlineUserAvatarBrush(FSlateBrush& Brush);

	UFUNCTION(BlueprintPure, Category = "BlueprintOnlineHelperUtils")
	static void GetOnlineFriendsList(TArray<FOnlineFriendInfo>& Friends);

	UFUNCTION(BlueprintPure, Category = "BlueprintOnlineHelperUtils")
	static void GetOnlineRecentlyFriendsList(TArray<FOnlineFriendInfo>& Friends);

	UFUNCTION(BlueprintPure, Category = "BlueprintOnlineHelperUtils")
	static void GetOnineSessionPlayersList(TArray<FOnlinePlayerInfo>& Players);

private:
	static EFriendOnlineState ConvertSteamFriendOnlineState(ESteamFriendOnlineState InState);
};
