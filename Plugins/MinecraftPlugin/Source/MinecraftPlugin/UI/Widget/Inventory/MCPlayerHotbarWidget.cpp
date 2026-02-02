// Developer: Jichao Luo


#include "MCPlayerHotbarWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCInventorySlotWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCInventoryItemWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCInventoryVisualizerWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCHotbarCursorWidget.h"
#include "MinecraftPlugin/Inventory/MCPlayerInventory.h"
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
	//WBP_MC_HotbarCursor->AnimateToNewParent(CreatedSlots[0]);
}

void UMCPlayerHotbarWidget::SpawnSlots()
{
}

void UMCPlayerHotbarWidget::UpdateCursorPosition()
{
}

void UMCPlayerHotbarWidget::UpdateItems()
{
}
