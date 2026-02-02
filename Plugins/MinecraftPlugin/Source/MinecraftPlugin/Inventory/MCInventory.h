// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MCInventoryInterface.h"
#include "MCInventory.generated.h"

class UMCItem;

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API UMCInventory : public UObject, public IMCInventoryInterface
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FIntVector2, UMCItem*> Items;

	FIntVector2 InventorySize;
	
public:
	UMCInventory();


	// IMCInventoryInterface Start~
protected:
	virtual void SetSize(const FIntVector2 Size) override;
public:
	virtual const FIntVector2 GetSize() const override;
	virtual bool InsertAnywhereStacked(UMCItem* Item) override;
	virtual bool InsertInto(const FIntVector2& Coord, UMCItem* Item) override;
	virtual bool RemoveFrom(const FIntVector2& Coord, UMCItem*& OutItem) override;
	virtual bool RemoveNumberFrom(const FIntVector2& Coord, const int32 NumToRemove, UMCItem*& OutItem) override;
	virtual bool HasItem(const FIntVector2& Coord, UMCItem*& OutItem) const override;
	// IMCInventoryInterface End~

protected:
	// 获取第一个元素相同且未堆满的物品格
	bool GetCoordOfNonFullStack(const UMCItem* Item, FIntVector2& OutCoord) const;

	// 获取第一个空的物品格
	bool GetFirstEmptyCoord(FIntVector2& OutCoord) const;

	FORCEINLINE bool IsCoordInBounds(const FIntVector2& Coord) const
	{
		return Coord.X < InventorySize.X && Coord.Y < InventorySize.Y && Coord.X >= 0 && Coord.Y >= 0;
	}

protected:
	FIntVector2 DefaultSize = FIntVector2(9, 4);

};
