// Copyright: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Definition/MultiplayerSessionsType.h"
#include "MultiplayerSessionsBPLibrary.generated.h"

class APlayerController;

/**
 * 
 */
UCLASS()
class SESSIONSSYSTEM_API UMultiplayerSessionsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static bool MultiplayerClientTravel(APlayerController* PlayerController, const FString& Address, ELevelTravelType TravelType);

	UFUNCTION(BlueprintCallable)
	static bool MultiplayerServerTravel(UObject* WorldContextObject, const FString& Address, bool bAbsolute = false, bool bShouldSkipGameNotify = false);

	UFUNCTION(BlueprintPure, meta = (ReturnDisplayName = "PlayerNumbers"))
	static int32 GetSessionPlayerNumbers(FName SessionName = FName("GameSession"));

	UFUNCTION(BlueprintPure, meta = (ReturnDisplayName = "SessionID"))
	static FString GetSessionID(FName SessionName = FName("GameSession"));

	UFUNCTION(BlueprintPure, meta = (ReturnDisplayName = "Params"))
	static FSessionsCreateParams GetSessionCreateParams();

	UFUNCTION(BlueprintPure, meta = (ReturnDisplayName = "PlayerNumber"))
	static int32 GetSessionCurrentPlayerNumber();
};
