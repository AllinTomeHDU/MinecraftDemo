#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "MultiplayerSessionsType.generated.h"


UENUM(BlueprintType)
enum class EGameEnterMode : uint8
{
	Startup,
	FromSession
};

UENUM(BlueprintType)
enum class ELevelTravelType : uint8
{
	TRAVEL_Absolute,	// 绝对路径，跨地图、跨服务器
	TRAVEL_Partial,		// 局部跳转，保留玩家名称等，同一服务器切换地图
	TRAVEL_Relative,	// 相对路径（相对于当前路径的URL解释）
	TRAVEL_MAX,
};

USTRUCT(BlueprintType)
struct SESSIONSSYSTEM_API FSessionsCreateParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RoomName = TEXT("XXX's Game");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DifficultyLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1", ClampMax = "5", UIMin = "1", UIMax = "5"))
	int32 MaxPlayers = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPublic = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Password;
};

USTRUCT(BlueprintType)
struct SESSIONSSYSTEM_API FSessionsSearchResult
{
	GENERATED_BODY()

	FOnlineSessionSearchResult SessionResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString SessionID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PingMs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString RoomName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentPlayers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 MaxPlayers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 DifficultyLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsPublic;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Password;
};

UENUM(BlueprintType)
enum class ESessionsJoinResult : uint8
{
	Success,
	SessionIsFull,
	SessionDoesNotExist,
	CouldNotRetrieveAddress,
	AlreadyInSession,
	UnknownError
};
