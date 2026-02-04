// Developer: Jichao Luo


#include "MCVoxelGameState.h"
#include "MinecraftPlugin/Inventory/MCPlayerInventory.h"
#include "MinecraftPlugin/Inventory/Item/MCItem.h"
#include "MinecraftPlugin/Inventory/Item/MCItemDatabase.h"


AMCVoxelGameState::AMCVoxelGameState()
{
	bReplicates = true;
}

void AMCVoxelGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InializePlayerInstoryData();
}

void AMCVoxelGameState::InializePlayerInstoryData()
{
	MainPlayerInventory = NewObject<UMCPlayerInventory>();
	GetInventoryDatabase().AddEntityInventory(TEXT("Player"), MainPlayerInventory);
	AddInitialItems(MainPlayerInventory);
}

void AMCVoxelGameState::AddInitialItems(TScriptInterface<IMCInventoryInterface> TargetInventory)
{
	for (auto Item : GetItemInfoDatabase()->GetItems())
	{
		auto NewItem = NewObject<UMCItem>();
		NewItem->Initialize(Item.Key, Item.Value.MaxStackSize);
		TargetInventory->InsertAnywhereStacked(NewItem);
	}
}

