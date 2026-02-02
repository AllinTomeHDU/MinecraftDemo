// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MinecraftPlugin/Library/Definition/MCPluginTypes.h"
#include "MCItemDatabase.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API UMCItemDatabase : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<FString, FMCItemInfo> Items;
	
public:
	FORCEINLINE const TMap<FString, FMCItemInfo>& GetItems() const { return Items; }

	FMCItemInfo GetItemInfo(const FString& ItemID) const;
	
	FString GetItemID(const EMCBlock Block) const;
};
