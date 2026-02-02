// Developer: Jichao Luo


#include "MCInventoryDatabase.h"


void UMCInventoryDatabase::Initialize()
{
	WorldInventories.Empty();
	EntityInventories.Empty();
}

void UMCInventoryDatabase::AddWorldInventory(const FIntVector Coord, const TScriptInterface<IMCInventoryInterface> Inventory)
{
	WorldInventories.Add(Coord, Inventory);
}

void UMCInventoryDatabase::AddEntityInventory(const FString ID, const TScriptInterface<IMCInventoryInterface> Inventory)
{
	EntityInventories.Add(ID, Inventory);
}

bool UMCInventoryDatabase::GetWorldInventory(const FIntVector& Coord, TScriptInterface<IMCInventoryInterface>& OutInventory) const
{
	if (!WorldInventories.Contains(Coord)) return false;

	OutInventory = WorldInventories[Coord];
	return true;
}

bool UMCInventoryDatabase::GetEntityInventory(const FString& ID, TScriptInterface<IMCInventoryInterface>& OutInventory) const
{
	if (!EntityInventories.Contains(ID)) return false;

	OutInventory = EntityInventories[ID];
	return true;
}

bool UMCInventoryDatabase::DoesWorldInventoryExist(const FIntVector& Coord) const
{
	return WorldInventories.Contains(Coord);
}

bool UMCInventoryDatabase::DoesEntityInventoryExist(const FString& ID) const
{
	return EntityInventories.Contains(ID);
}
