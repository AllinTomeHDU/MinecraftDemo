// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "../MCUserWidgetBase.h"
#include "MCInventoryGridWidget.generated.h"

class IMCInventoryInterface;
class UMCPlayerInventory;
class UMCInventorySlotWidget;
class UMCInventoryItemWidget;
class UMCInventoryVisualizerWidget;

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API UMCInventoryGridWidget : public UMCUserWidgetBase
{
	GENERATED_BODY()
	
public:
	void SpawnInventoryGrid(
		TScriptInterface<IMCInventoryInterface> Inventory,
		UMCInventoryVisualizerWidget* InventoryVisualizer,
		TSubclassOf<UMCInventorySlotWidget> InSlotWidgetClass,
		TSubclassOf<UMCInventoryItemWidget> InItemWidgetClass,
		FIntVector2 SizeOffset = {0, 0}
	);
	
private:
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* WrapBox_InventorySlot;

public:
	FORCEINLINE UPanelWidget* GetWrapBox_InventorySlot() const { return WrapBox_InventorySlot; }
};
