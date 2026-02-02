// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MinecraftPlugin/Inventory/MCInventoryDatabase.h"
#include "MCVoxelGameState.generated.h"

class UMCItemDatabase;
class UMCPlayerInventory;
class UMCInventoryVisualizerWidget;

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API AMCVoxelGameState : public AGameState
{
	GENERATED_BODY()
	
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
	FORCEINLINE UMCInventoryDatabase& GetInventoryDatabase();

	FORCEINLINE UMCPlayerInventory* GetPlayerInventory() const { return MainPlayerInventory; }
	FORCEINLINE void SetPlayerInventory(UMCPlayerInventory* NewInventory) { MainPlayerInventory = NewInventory; }

	FORCEINLINE UMCInventoryVisualizerWidget* GetInventoryVisualizer() const { return InventoryVisualizer; }
	FORCEINLINE void SetInventoryVisualizer(UMCInventoryVisualizerWidget* NewWidget) { InventoryVisualizer = NewWidget; }

};
