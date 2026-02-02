// Developer: Jichao Luo


#include "MCInventory.h"
#include "Item/MCItem.h"
#include "Item/MCItemDatabase.h"
#include "MinecraftPlugin/Game/MCVoxelGameState.h"


UMCInventory::UMCInventory()
{
	InventorySize = DefaultSize;
}

void UMCInventory::SetSize(FIntVector2 Size)
{
	// 缩小背包尺寸之前检测当前背包中是否有物品超出了新的尺寸范围
	for (auto ItemTuple : Items)
	{
		if (ItemTuple.Key.X > Size.X || ItemTuple.Key.Y > Size.Y)
		{
			// TODO: “缩小背包”时的自动清理或转移逻辑
			GLog->Log(ELogVerbosity::Error, TEXT("[Inventory::SetSize]: Decrease of inventory size is currently unsupported."));
			return;
		}
	}

	InventorySize = Size;
}

const FIntVector2 UMCInventory::GetSize() const
{
	return InventorySize;
}

bool UMCInventory::InsertAnywhereStacked(UMCItem* Item)
{
	/**
	* 将一个物品堆（Item）插入到背包中：
	* 	如果已有同类但未满堆的格子 → 继续往该堆叠加；
	* 	如果没有非满堆但有空格子 → 新建一堆；
	* 	如果都没有 → 返回 false（背包已满）。
	*/
	int32 NumToInsert = Item->GetCurrentStack();
	while (NumToInsert > 0)
	{
		FIntVector2 NonFullStackCoord;
		FIntVector2 FirstEmptyCoord;
		if (GetCoordOfNonFullStack(Item, NonFullStackCoord))
		{
			auto ItemDatabase = Cast<AMCVoxelGameState>(GEngine->GetCurrentPlayWorld()->GetGameState())->GetItemInfoDatabase();
			auto CurrentItemInfo = ItemDatabase->GetItemInfo(Item->GetAssociatedItemID());
			auto NonFullStackedItem = Items[NonFullStackCoord];

			int32 MaxToInsert = CurrentItemInfo.MaxStackSize - NonFullStackedItem->GetCurrentStack();
			int32 SubtotalToInsert = FMath::Min(NumToInsert, MaxToInsert);
			if (!NonFullStackedItem->SetCurrentStackUnChecked(NonFullStackedItem->GetCurrentStack() + SubtotalToInsert))
			{
				GLog->Log(ELogVerbosity::Error, TEXT("[Inventory::InsertAnywhere]: Unable to check stack size. Stack size will not be modified."));
				return false;
			}
			NumToInsert -= SubtotalToInsert;
		}
		else if (GetFirstEmptyCoord(FirstEmptyCoord))
		{
			// TODO: Stack最大值限制
			auto NewlyStackedItem = NewObject<UMCItem>();
			NewlyStackedItem->Initialize(Item->GetAssociatedItemID(), NumToInsert);
			Items.Add(FirstEmptyCoord, NewlyStackedItem);
			NumToInsert = 0;
		}
		else
		{
			return false;
		}
	}
	return NumToInsert == 0;
}

bool UMCInventory::InsertInto(const FIntVector2& Coord, UMCItem* Item)
{
	if (!IsCoordInBounds(Coord) || Items.Contains(Coord)) return false;

	Items.Add(Coord, Item);
	return true;
}

bool UMCInventory::RemoveFrom(const FIntVector2& Coord, UMCItem*& OutItem)
{
	UMCItem* RemovedItem;
	auto ReturnValue = Items.RemoveAndCopyValue(Coord, RemovedItem);
	OutItem = RemovedItem;
	return ReturnValue;
}

bool UMCInventory::RemoveNumberFrom(const FIntVector2& Coord, const int32 NumToRemove, UMCItem*& OutItem)
{
	UMCItem* HasItemOut;
	if (!HasItem(Coord, HasItemOut)) return false;
	if (HasItemOut->GetCurrentStack() < NumToRemove) return false;
	if (HasItemOut->GetCurrentStack() == NumToRemove) return RemoveFrom(Coord, OutItem);

	// update current stack size
	if (!HasItemOut->SetCurrentStackUnChecked(HasItemOut->GetCurrentStack() - NumToRemove)) return false;

	OutItem = NewObject<UMCItem>();
	OutItem->Initialize(HasItemOut->GetAssociatedItemID(), NumToRemove);
	return true;
}

bool UMCInventory::HasItem(const FIntVector2& Coord, UMCItem*& OutItem) const
{
	if (!IsCoordInBounds(Coord) || !Items.Contains(Coord)) return false;

	OutItem = Items[Coord];
	return true;
}

bool UMCInventory::GetCoordOfNonFullStack(const UMCItem* Item, FIntVector2& OutCoord) const
{
	if (!GEngine || !GEngine->GetCurrentPlayWorld()) return false;
	for (int32 x = 0; x < GetSize().X; x++)
	{
		for (int32 y = 0; y < GetSize().Y; y++)
		{
			FIntVector2 CurrentCoord(x, y);
			if (!Items.Contains(CurrentCoord)) continue;
			if (Items[CurrentCoord]->GetAssociatedItemID() != Item->GetAssociatedItemID()) continue;

			auto CurrentItemInfo = Cast<AMCVoxelGameState>(GEngine->GetCurrentPlayWorld()->GetGameState())
				->GetItemInfoDatabase()->GetItemInfo(Item->GetAssociatedItemID());

			if (Items[CurrentCoord]->GetCurrentStack() >= CurrentItemInfo.MaxStackSize) continue;

			OutCoord = CurrentCoord;
			return true;
		}
	}
	return false;
}

bool UMCInventory::GetFirstEmptyCoord(FIntVector2& OutCoord) const
{
	for (int32 x = 0; x < GetSize().X; x++)
	{
		for (int32 y = 0; y < GetSize().Y; y++)
		{
			FIntVector2 CurrentCoord(x, y);
			if (!Items.Contains(CurrentCoord))
			{
				OutCoord = CurrentCoord;
				return true;
			}
		}
	}
	return false;
}
