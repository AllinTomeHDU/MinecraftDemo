// Developer: Jichao Luo


#include "MCInventorySlotWidget.h"
#include "MCInventoryItemWidget.h"
#include "MCInventoryVisualizerWidget.h"
#include "MinecraftPlugin/Inventory/MCInventoryInterface.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"


FIntVector2 UMCInventorySlotWidget::GetRepresentedInventoryCoord() const
{
	return FIntVector2();
}

void UMCInventorySlotWidget::InitializeData(
	FIntVector2 InRepresentedInventoryCoord, 
	UMCInventoryVisualizerWidget* InOwningInventoryVisualizer, 
	TScriptInterface<IMCInventoryInterface> InAssociatedInventory)
{
	RepresentedInventoryCoord = InRepresentedInventoryCoord;
	OwningInventoryVisualizer = InOwningInventoryVisualizer;
	AssociatedInventory = InAssociatedInventory;
}

void UMCInventorySlotWidget::AddItemWidget(UMCInventoryItemWidget* WidgetToAdd)
{
	Overlay_Item->AddChild(WidgetToAdd);
	CurrentItemWidget = WidgetToAdd;
}

bool UMCInventorySlotWidget::RemoveCurrentItemWidget()
{
	if (!IsValid(CurrentItemWidget)) return false;

	CurrentItemWidget->RemoveFromParent();
	CurrentItemWidget = nullptr;
	return true;
}

void UMCInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Slot->OnClicked.AddDynamic(this, &ThisClass::OnButtonClicked);
}

void UMCInventorySlotWidget::OnButtonClicked()
{
	OwningInventoryVisualizer->OnSlotButtonClicked(this);
}
