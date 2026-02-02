// Developer: Jichao Luo


#include "MCVoxelGameMode.h"
#include "MCVoxelGameState.h"
#include "MinecraftPlugin/Inventory/MCPlayerInventory.h"
#include "MinecraftPlugin/Inventory/Item/MCItem.h"
#include "MinecraftPlugin/Inventory/Item/MCItemDatabase.h"


void AMCVoxelGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (AMCVoxelGameState* GS = Cast<AMCVoxelGameState>(GetWorld()->GetGameState()))
	{
		auto CreatedPlayerInventory = NewObject<UMCPlayerInventory>();
		GS->SetPlayerInventory(CreatedPlayerInventory);
		GS->GetInventoryDatabase().AddEntityInventory(TEXT("Player"), CreatedPlayerInventory);
		AddInitialItems(CreatedPlayerInventory);
	}
}

void AMCVoxelGameMode::AddInitialItems(TScriptInterface<IMCInventoryInterface> TargetInventory)
{
	if (AMCVoxelGameState* GS = Cast<AMCVoxelGameState>(GetWorld()->GetGameState()))
	{
		for (auto Item : GS->GetItemInfoDatabase()->GetItems())
		{
			auto NewItem = NewObject<UMCItem>();
			NewItem->Initialize(Item.Key, Item.Value.MaxStackSize);
			TargetInventory->InsertAnywhereStacked(NewItem);
		}
	}
}
