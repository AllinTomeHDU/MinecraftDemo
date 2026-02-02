// Developer: Jichao Luo


#include "MCItem.h"
#include "MCItemDatabase.h"


void UMCItem::Initialize(const FString& InAssociatedItemID, const int32 CurrentStackSize)
{
	AssociatedItemID = InAssociatedItemID;
	CurrentStack = CurrentStackSize;
}

bool UMCItem::SetCurrentStackChecked(int32 NewStack, const UMCItemDatabase* ItemInfoDB)
{
	if (!ItemInfoDB || NewStack <= 0) return false;
	if (NewStack > ItemInfoDB->GetItemInfo(AssociatedItemID).MaxStackSize) return false;

	CurrentStack = NewStack;
	return true;
}

bool UMCItem::SetCurrentStackUnChecked(int32 NewStack)
{
	if (NewStack <= 0) return false;

	CurrentStack = NewStack;
	return true;
}
