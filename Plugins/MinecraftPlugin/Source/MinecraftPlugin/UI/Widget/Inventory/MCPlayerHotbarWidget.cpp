// Developer: Jichao Luo


#include "MCPlayerHotbarWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCInventorySlotWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCInventoryItemWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCInventoryVisualizerWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCHotbarCursorWidget.h"
#include "MinecraftPlugin/Inventory/MCPlayerInventory.h"
#include "MinecraftPlugin/Inventory/Item/MCItem.h"
#include "MinecraftPlugin/Game/MCVoxelGameState.h"
#include "Components/PanelWidget.h"


void UMCPlayerHotbarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (auto GS = Cast<AMCVoxelGameState>(GetWorld()->GetGameState()))
	{
		RepresentedPlayerInventory = GS->GetPlayerInventory();
		InventoryVisualizerWidget = GS->GetInventoryVisualizer();
	}

	SpawnSlots();
	UpdateItems();
	WBP_MC_HotbarCursor->AnimateToNewParent(CreatedSlots[0]->GetHotbarCursorParent());
}

void UMCPlayerHotbarWidget::UpdateCursorPosition()
{
	int32 TargetSelectedSlot = RepresentedPlayerInventory->GetCurrentlySelectedHotbarSlot();

	if (TargetSelectedSlot < 0 || TargetSelectedSlot > CreatedSlots.Num() - 1)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UPlayerHotbarWidget::UpdateCursorPosition]: Arg error, TargetSelectedSlot is out of bounds. Cursor position will not be updated."));
		return;
	}
	if (CreatedSlots[TargetSelectedSlot]->GetHotbarCursorParent() == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UPlayerHotbarWidget::UpdateCursorPosition]: The selected slot does not have a HotbarCursorParent set. Cursor position will not be updated."));
		return;
	}

	WBP_MC_HotbarCursor->AnimateToNewParent(CreatedSlots[TargetSelectedSlot]->GetHotbarCursorParent());
}

void UMCPlayerHotbarWidget::UpdateItems()
{
	if (CreatedSlots.IsEmpty()) return;

	int32 y = RepresentedPlayerInventory->GetHotBarRowStartCoords().Y;
	for (int32 x = RepresentedPlayerInventory->GetHotBarRowStartCoords().X; x < RepresentedPlayerInventory->GetHotbarSize().X; x++)
	{
		UMCInventorySlotWidget* CurrentSlot = CreatedSlots[x];
		if (CurrentSlot->GetCurrentItemWidget())
		{
			CurrentSlot->RemoveCurrentItemWidget(); // dispose of current item.
		}

		UMCItem* Item;
		if (RepresentedPlayerInventory->HasItem(FIntVector2(x, y), Item))
		{
			auto NewItemWidget = CreateWidget<UMCInventoryItemWidget>(GetWorld(), ItemWidgetClass);
			CurrentSlot->AddItemWidget(NewItemWidget);
			NewItemWidget->SetRepresentedItem(Item);
		}
	}
}

void UMCPlayerHotbarWidget::SpawnSlots()
{
	if (!IsValid(RepresentedPlayerInventory) || !IsValid(InventoryVisualizerWidget)) return;

	if (!CreatedSlots.IsEmpty())
	{
		CreatedSlots.Empty();
		WrapBox_SlotParent->ClearChildren();
	}
	CreatedSlots.Reserve(RepresentedPlayerInventory->GetHotbarSize().X);

	int32 y = RepresentedPlayerInventory->GetHotBarRowStartCoords().Y;
	for (int32 x = RepresentedPlayerInventory->GetHotBarRowStartCoords().X; x < RepresentedPlayerInventory->GetHotbarSize().X; x++)
	{
		auto NewSlotWidget = CreateWidget<UMCInventorySlotWidget>(GetWorld(), SlotWidgetClass);
		NewSlotWidget->InitializeData(FIntVector2(x, y), InventoryVisualizerWidget, RepresentedPlayerInventory);
		WrapBox_SlotParent->AddChild(NewSlotWidget);
		CreatedSlots.Add(NewSlotWidget);
	}
}
