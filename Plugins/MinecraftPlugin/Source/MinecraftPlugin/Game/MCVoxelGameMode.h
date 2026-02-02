// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MCVoxelGameMode.generated.h"

class IMCInventoryInterface;

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API AMCVoxelGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	void AddInitialItems(TScriptInterface<IMCInventoryInterface> TargetInventory);
	
};
