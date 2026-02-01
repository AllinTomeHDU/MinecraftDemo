// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MCVoxelUtilsLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API UMCVoxelUtilsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static FIntVector WorldToBlockPosition(const FVector& WorldPosition);

	static FIntVector WorldToLocalBlockPosition(const FVector& Position, const FIntVector& ChunkSize);

	static FIntVector WorldToChunkPosition(const FVector& Position, const FIntVector& ChunkSize);
	
};
