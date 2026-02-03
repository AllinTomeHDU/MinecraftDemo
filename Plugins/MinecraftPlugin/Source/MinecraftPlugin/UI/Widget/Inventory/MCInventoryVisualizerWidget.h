// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "../MCUserWidgetBase.h"
#include "MinecraftPlugin/Library/Definition/MCPluginTypes.h"
#include "MCInventoryVisualizerWidget.generated.h"

class UMCInventorySlotWidget;
class UMCInventoryItemWidget;
class UMCInventoryGridWidget;
class IMCInventoryInterface;
class UMCPlayerInventory;
class UButton;

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API UMCInventoryVisualizerWidget : public UMCUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void TogglePlayerInventory(UMCPlayerInventory* InPlayerInventory, bool& OutIsMenuDisplayed);

	UFUNCTION(BlueprintCallable)
	void ToggleBothInventories(UMCPlayerInventory* InPlayerInventory, TScriptInterface<IMCInventoryInterface> InOtherInventory, bool& OutIsMenuDisplayed);

	UFUNCTION()
	void OnSlotButtonClicked(UMCInventorySlotWidget* InSlotWidget);

	UFUNCTION()
	void OnCloseButtonClicked();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION(BlueprintCallable)
	void ChangePlayerInputsToWorld();

private:
	void Hide();
	void HideSecondaryInventory();
	void HidePlayerInventory();

	void InitPlayerInventoryWidget(UMCPlayerInventory* InPlayerInventory);
	void InitOtherInventoryWidget(TScriptInterface<IMCInventoryInterface> InOtherInventory);

	void TrackCurrentlyHeldItem();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Minecraft")
	TSubclassOf<UMCInventorySlotWidget> InventorySlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Blueprints")
	TSubclassOf<UMCInventoryItemWidget> InventoryItemWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Close;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* Overlay_OtherInventory;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* Overlay_PlayerInventory;

	UPROPERTY(meta = (BindWidget))
	UMCInventoryGridWidget* InventoryGrid_Other;

	UPROPERTY(meta = (BindWidget))
	UMCInventoryGridWidget* InventoryGrid_Player;

	UPROPERTY()
	TScriptInterface<IMCInventoryInterface> CurrentOtherInventory;

	UPROPERTY()
	UMCPlayerInventory* CurrentPlayerInventory;

	UPROPERTY()
	UMCInventoryItemWidget* CurrentHeldItem;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* Overlay_HeldItem;

	EMCInventoryVisualiserState VisualiserState = EMCInventoryVisualiserState::Hidden;
	const ESlateVisibility DefaultSelfInvisibility = ESlateVisibility::Hidden;
	const ESlateVisibility DefaultSelfVisibility = ESlateVisibility::SelfHitTestInvisible; 
	
};
