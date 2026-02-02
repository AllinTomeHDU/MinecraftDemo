// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "../MCUserWidgetBase.h"
#include "MCPlayerHotbarWidget.generated.h"

class UMCInventorySlotWidget;
class UMCInventoryItemWidget;
class UMCInventoryVisualizerWidget;
class UMCHotbarCursorWidget;
class UMCPlayerInventory;

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API UMCPlayerHotbarWidget : public UMCUserWidgetBase
{
	GENERATED_BODY()
	
public:
	void UpdateCursorPosition();
	void UpdateItems();
	
protected:
	virtual void NativeOnInitialized() override;

private:
	void SpawnSlots();

private:
	UPROPERTY(Transient, VisibleAnywhere, meta = (BindWidget))
	UMCHotbarCursorWidget* WBP_MC_HotbarCursor;

	UPROPERTY(Transient, VisibleAnywhere, meta = (BindWidget))
	UPanelWidget* WrapBox_SlotParent;

	UPROPERTY(EditDefaultsOnly, Category = "Minecraft")
	TSubclassOf<UMCInventorySlotWidget> InventorySlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Minecraft")
	TSubclassOf<UMCInventoryItemWidget> InventoryItemWidgetClass;

	UPROPERTY()
	UMCPlayerInventory* RepresentedPlayerInventory;

	UPROPERTY()
	UMCInventoryVisualizerWidget* InventoryVisualizerWidget;

	UPROPERTY()
	TArray<UMCInventorySlotWidget*> CreatedSlots;
};
