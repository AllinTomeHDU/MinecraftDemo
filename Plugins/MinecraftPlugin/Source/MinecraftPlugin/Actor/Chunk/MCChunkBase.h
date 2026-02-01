// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinecraftPlugin/Library/Definition/MCPluginTypes.h"
#include "MCChunkBase.generated.h"

class UMaterialInterface;
class AMCChunkManager;
class UProceduralMeshComponent;
class FastNoise;

UCLASS()
class MINECRAFTPLUGIN_API AMCChunkBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMCChunkBase();

	// Call by ChunkManager
	void Initialise(AMCChunkManager* InChunkManager, FastNoise* InNoiseGen, const FIntVector InChunkSize, const FIntVector2 InChunkLocation);

	// Modify the block at the current position
	virtual void ModifyVoxel(const FIntVector Position, const EMCBlock NewBlock) {};

	virtual EMCBlock GetBlock(const FIntVector& Coord) const { return EMCBlock::Null; }

	UPROPERTY()
	UMaterialInterface* MaterialInterface;

protected:
	virtual void BeginPlay() override;

	virtual void OnInitialiseComplete() {};

protected:
	UPROPERTY(VisibleInstanceOnly, Category = "Chunk")
	AMCChunkManager* ChunkManager;

	UPROPERTY(VisibleInstanceOnly, Category = "Chunk")
	FIntVector ChunkSize;

	UPROPERTY(VisibleInstanceOnly, Category = "Chunk")
	FIntVector2 ChunkLocation;

	UPROPERTY()
	UProceduralMeshComponent* Mesh;

	FastNoise* NoiseGenerator;
	
public:
	FORCEINLINE FIntVector2 GetChunkLocation() const { return ChunkLocation; }
	FORCEINLINE FIntVector GetChunkSize() const { return ChunkSize; }
	FORCEINLINE AMCChunkManager* GetChunkManager() const { return ChunkManager; }
};
