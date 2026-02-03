// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "MCInventory.h"
#include "MCPlayerInventory.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API UMCPlayerInventory : public UMCInventory
{
	GENERATED_BODY()
	
	FIntVector2 HotbarSize = FIntVector2(9, 1);

	int32 CurrentlySelectedHotbarSlot = 0;
	
public:
	UMCPlayerInventory();

	FORCEINLINE FIntVector2 GetHotBarRowStartCoords() const { return FIntVector2(0, GetSize().Y - 1); }
	FORCEINLINE FIntVector2 GetHotbarSize() const { return HotbarSize; }
	FORCEINLINE int32 GetCurrentlySelectedHotbarSlot() const { return CurrentlySelectedHotbarSlot; }
	FORCEINLINE bool SetCurrentlySelectedHotbarSlot(int32 NewlySelectedSlot)
	{
		if (NewlySelectedSlot < 0 || NewlySelectedSlot > HotbarSize.X - 1) return false;
		CurrentlySelectedHotbarSlot = NewlySelectedSlot;
		return true;
	}
	FORCEINLINE FIntVector2 GetCurrentlySelectedHotbarSlotCoords() const
	{
		auto StartCoords = GetHotBarRowStartCoords();
		StartCoords.X = GetCurrentlySelectedHotbarSlot();
		return StartCoords;
	}

};
