// Copyright: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerRoomGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SESSIONSSYSTEM_API AMultiplayerRoomGameMode : public AGameMode
{
	GENERATED_BODY()

	UPROPERTY()
	class UMultiplayerSessionsSubsystem* SessionsSubsystem;
	
public:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
};
