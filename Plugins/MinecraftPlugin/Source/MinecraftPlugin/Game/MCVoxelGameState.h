// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MinecraftPlugin/Inventory/MCInventoryDatabase.h"
#include "MCVoxelGameState.generated.h"

class UMCItemDatabase;
class UMCPlayerInventory;
class UMCInventoryVisualizerWidget;
class IMCInventoryInterface;

DECLARE_MULTICAST_DELEGATE(FOnPlayerInventoryReady);

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API AMCVoxelGameState : public AGameState
{
	GENERATED_BODY()

public:
	AMCVoxelGameState();

	FOnPlayerInventoryReady OnPlayerInventoryReadyDelegate;

	void InializePlayerInstoryData();

protected:
	void AddInitialItems(TScriptInterface<IMCInventoryInterface> TargetInventory);

protected:
	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Minecraft")
	UMCItemDatabase* ItemInfoDatabase;

	UPROPERTY()
	UMCInventoryDatabase* MainInventoryDatabase;

	UPROPERTY()
	UMCPlayerInventory* MainPlayerInventory;

	UPROPERTY()
	UMCInventoryVisualizerWidget* InventoryVisualizer;
	
public:
	FORCEINLINE UMCItemDatabase* GetItemInfoDatabase() const { return ItemInfoDatabase; }
	FORCEINLINE UMCInventoryDatabase& GetInventoryDatabase()
	{
		if (!IsValid(MainInventoryDatabase))
		{
			MainInventoryDatabase = NewObject<UMCInventoryDatabase>();
			MainInventoryDatabase->Initialize();
		}
		return *MainInventoryDatabase;
	}

	FORCEINLINE UMCPlayerInventory* GetPlayerInventory() const { return MainPlayerInventory; }
	FORCEINLINE void SetPlayerInventory(UMCPlayerInventory* NewInventory) { MainPlayerInventory = NewInventory; }

	FORCEINLINE UMCInventoryVisualizerWidget* GetInventoryVisualizer() const { return InventoryVisualizer; }
	FORCEINLINE void SetInventoryVisualizer(UMCInventoryVisualizerWidget* NewWidget) { InventoryVisualizer = NewWidget; }

};
