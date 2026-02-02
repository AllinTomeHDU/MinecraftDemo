// Developer: Jichao Luo


#include "MCInventoryGridWidget.h"
#include "MCInventorySlotWidget.h"
#include "MCInventoryItemWidget.h"
#include "MinecraftPlugin/Inventory/MCInventoryInterface.h"
#include "MinecraftPlugin/Inventory/Item/MCItem.h"
#include "Components/PanelWidget.h"


void UMCInventoryGridWidget::SpawnInventoryGrid(
	TScriptInterface<IMCInventoryInterface> Inventory, 
	UMCInventoryVisualizerWidget* InventoryVisualizer,
	TSubclassOf<UMCInventorySlotWidget> InSlotWidgetClass, 
	TSubclassOf<UMCInventoryItemWidget> InItemWidgetClass, 
	FIntVector2 SizeOffset)
{
	for (int32 x = 0; x < Inventory->GetSize().X + SizeOffset.X; x++)
	{
		for (int32 y = 0; y < Inventory->GetSize().Y + SizeOffset.Y; y++)
		{
			auto NewSlotWidget = CreateWidget<UMCInventorySlotWidget>(GetWorld(), InSlotWidgetClass);
			NewSlotWidget->InitializeData(FIntVector2(x, y), InventoryVisualizer, Inventory);
			WrapBox_InventorySlot->AddChild(NewSlotWidget);

			UMCItem* Item;
			if (Inventory->HasItem(FIntVector2(x, y), Item))
			{
				auto NewItemWidget = CreateWidget<UMCInventoryItemWidget>(GetWorld(), InItemWidgetClass);
				NewSlotWidget->AddItemWidget(NewItemWidget);
				NewItemWidget->SetRepresentedItem(Item);
			}
		}
	}
}
