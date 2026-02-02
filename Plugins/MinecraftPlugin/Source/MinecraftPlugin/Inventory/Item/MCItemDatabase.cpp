// Developer: Jichao Luo


#include "MCItemDatabase.h"


FMCItemInfo UMCItemDatabase::GetItemInfo(const FString& ItemID) const
{
	return Items.Contains(ItemID) ? Items[ItemID] : Items.Array()[0].Value;
}

FString UMCItemDatabase::GetItemID(const EMCBlock Block) const
{
	for (auto& Item : Items)
	{
		if (Item.Value.BlockType == Block) return Item.Key;
	}
	return Items.Array()[0].Key;
}
