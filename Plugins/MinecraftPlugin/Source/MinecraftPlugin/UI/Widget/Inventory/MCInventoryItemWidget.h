// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "../MCUserWidgetBase.h"
#include "MCInventoryItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API UMCInventoryItemWidget : public UMCUserWidgetBase
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_ItemIcon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_StackNumber;

	UPROPERTY()
	class UMCItem* RepresentedItem;

public:
	FORCEINLINE UMCItem* GetRepresentedItem() const { return RepresentedItem; }

	void SetRepresentedItem(UMCItem* NewItem);
};
