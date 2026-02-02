// Developer: Jichao Luo


#include "MCInventoryItemWidget.h"
#include "MinecraftPlugin/Inventory/Item/MCItem.h"
#include "MinecraftPlugin/Inventory/Item/MCItemDatabase.h"
#include "MinecraftPlugin/Game/MCVoxelGameState.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UMCInventoryItemWidget::SetRepresentedItem(UMCItem* NewItem)
{
	RepresentedItem = NewItem;

	if (auto GS = Cast<AMCVoxelGameState>(GetWorld()->GetGameState()))
	{
		auto ItemInfoDB = GS->GetItemInfoDatabase();
		check(ItemInfoDB);
		auto RepresentedItemInfo = ItemInfoDB->GetItemInfo(NewItem->GetAssociatedItemID());
		Image_ItemIcon->SetBrushFromTexture(RepresentedItemInfo.ItemThumbnail);
		Text_StackNumber->SetText(FText::AsNumber(NewItem->GetCurrentStack()));
	}
}
