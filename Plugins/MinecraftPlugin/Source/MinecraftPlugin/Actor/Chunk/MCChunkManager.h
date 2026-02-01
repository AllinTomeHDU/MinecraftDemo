// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MCChunkBase.h"
#include "MinecraftPlugin/Library/Definition/MCPluginTypes.h"
#include "MCChunkManager.generated.h"

class FastNoise;
class UMaterialInterface;

UCLASS()
class MINECRAFTPLUGIN_API AMCChunkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AMCChunkManager();

	// 给定一个区块和一个方向，查找该方向上的相邻区块，如果存在则返回 true 并输出它
	AMCChunkBase* GetAdjacentChunk(AMCChunkBase* CurrentChunk, EMCDirection Direction);

protected:
	virtual void BeginPlay() override;

	void ConfigureNoiseGenerator();
	void GenerateInitialChunks();

private:
	UPROPERTY(EditAnywhere, Category = "Chunk Manager")
	TSubclassOf<AMCChunkBase> ChunksToSpawn = AMCChunkBase::StaticClass();
	
	UPROPERTY(EditAnywhere, Category = "Chunk Manager")
	int32 DrawDistanceAroundPlayer = 5;

	UPROPERTY(EditAnywhere, Category = "Chunk Manager")
	FIntVector NewChunkSize = FIntVector(32, 32, 32);

	UPROPERTY(EditAnywhere, Category = "Chunk Manager")
	UMaterialInterface* ChunkMaterialInterface;

	FastNoise* NoiseGenerator;

	static const FName DefaultChunkTag;

	TMap<FIntVector2, TObjectPtr<AMCChunkBase>> Chunks;

public:
	FORCEINLINE UMaterialInterface* GetChunkMaterialInterface() const { return ChunkMaterialInterface; }
	FORCEINLINE FIntVector GetNewChunkSize() const { return NewChunkSize; }
	FORCEINLINE FastNoise* GetNoiseGenerator() const { return NoiseGenerator; }
	FORCEINLINE const FName& GetDefaultChunkTag() const { return DefaultChunkTag; }

};
