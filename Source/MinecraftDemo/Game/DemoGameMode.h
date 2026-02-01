// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DemoGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFTDEMO_API ADemoGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};
