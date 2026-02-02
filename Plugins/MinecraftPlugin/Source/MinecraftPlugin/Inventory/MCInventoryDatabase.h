// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MCInventoryInterface.h"
#include "MCInventoryDatabase.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API UMCInventoryDatabase : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY()
	TMap<FIntVector, TScriptInterface<IMCInventoryInterface>> WorldInventories;

	UPROPERTY()
	TMap<FString, TScriptInterface<IMCInventoryInterface>> EntityInventories;

public:
	void Initialize();
	
	void AddWorldInventory(const FIntVector Coord, const TScriptInterface<IMCInventoryInterface> Inventory);
	void AddEntityInventory(const FString ID, const TScriptInterface<IMCInventoryInterface> Inventory);

	bool GetWorldInventory(const FIntVector& Coord, TScriptInterface<IMCInventoryInterface>& OutInventory) const;
	bool GetEntityInventory(const FString& ID, TScriptInterface<IMCInventoryInterface>& OutInventory) const;

	bool DoesWorldInventoryExist(const FIntVector& Coord) const;
	bool DoesEntityInventoryExist(const FString& ID) const;
};
