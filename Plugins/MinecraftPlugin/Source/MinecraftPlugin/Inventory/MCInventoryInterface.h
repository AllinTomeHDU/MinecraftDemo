// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MCInventoryInterface.generated.h"

class UMCItem;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMCInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MINECRAFTPLUGIN_API IMCInventoryInterface
{
	GENERATED_BODY()

protected:
	virtual void SetSize(const FIntVector2 Size) = 0;

public:
	virtual const FIntVector2 GetSize() const = 0;
	
	// ÎïÆ·¶ÑµþÂß¼­
	virtual bool InsertAnywhereStacked(UMCItem* Item) = 0;

	// Insert an item into a specific coordiante that must be empty
	virtual bool InsertInto(const FIntVector2& Coord, UMCItem* Item) = 0;

	// Remove an item from a specific coordinate, if possible
	virtual bool RemoveFrom(const FIntVector2& Coord, UMCItem*& OutItem) = 0;

	// Removes a certain amount of items from a coordiante, if possible
	virtual bool RemoveNumberFrom(const FIntVector2& Coord, const int32 NumToRemove, UMCItem*& OutItem) = 0;

	// Check whether the inventory has the item, and if so returns a reference to the item
	virtual bool HasItem(const FIntVector2& Coord, UMCItem*& OutItem) const = 0;
};
