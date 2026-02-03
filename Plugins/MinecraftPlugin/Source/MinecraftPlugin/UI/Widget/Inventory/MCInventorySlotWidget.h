// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "../MCUserWidgetBase.h"
#include "MCInventorySlotWidget.generated.h"

class IMCInventoryInterface;
class UMCInventoryVisualizerWidget;
class UMCInventoryItemWidget;

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API UMCInventorySlotWidget : public UMCUserWidgetBase
{
	GENERATED_BODY()

public:
	FIntVector2 GetRepresentedInventoryCoord() const;

	void InitializeData(
		FIntVector2 InRepresentedInventoryCoord,
		UMCInventoryVisualizerWidget* InOwningInventoryVisualizer,
		TScriptInterface<IMCInventoryInterface> InAssociatedInventory
	);

	void AddItemWidget(UMCInventoryItemWidget* WidgetToAdd);
	bool RemoveCurrentItemWidget();

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* Overlay_HotbarCursor;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* Overlay_Item;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Slot;

	UPROPERTY()
	class UMCInventoryVisualizerWidget* OwningInventoryVisualizer;

	UPROPERTY()
	class UMCInventoryItemWidget* CurrentItemWidget;
	
	TScriptInterface<IMCInventoryInterface> AssociatedInventory;

	FIntVector2 RepresentedInventoryCoord;

public:
	FORCEINLINE UPanelWidget* GetHotbarCursorParent() const { return Overlay_HotbarCursor; }
	FORCEINLINE UMCInventoryItemWidget* GetCurrentItemWidget() const { return CurrentItemWidget; }
	FORCEINLINE TScriptInterface<IMCInventoryInterface> GetAssociatedInventory() const { return AssociatedInventory; }
	FORCEINLINE bool IsHoldingWidget() const { return GetCurrentItemWidget() != nullptr; }
};
