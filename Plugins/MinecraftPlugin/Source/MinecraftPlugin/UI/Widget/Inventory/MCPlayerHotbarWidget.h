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
	bool SpawnSlots();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Minecraft")
	TSubclassOf<UMCInventorySlotWidget> SlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Minecraft")
	TSubclassOf<UMCInventoryItemWidget> ItemWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UMCHotbarCursorWidget* WBP_MC_HotbarCursor;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* WrapBox_SlotParent;

	UPROPERTY()
	TArray<UMCInventorySlotWidget*> CreatedSlots;

	UPROPERTY()
	UMCPlayerInventory* RepresentedPlayerInventory;

	UPROPERTY()
	UMCInventoryVisualizerWidget* InventoryVisualizerWidget;


};
