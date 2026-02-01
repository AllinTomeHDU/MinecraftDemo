// Developer: Jichao Luo


#include "MCChunkManager.h"
#include "FastNoise.h"


const FName AMCChunkManager::DefaultChunkTag = TEXT("VoxelChunk");

AMCChunkManager::AMCChunkManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AMCChunkManager::BeginPlay()
{
	Super::BeginPlay();

	ConfigureNoiseGenerator();
	GenerateInitialChunks();
}

void AMCChunkManager::ConfigureNoiseGenerator()
{
	NoiseGenerator = new FastNoise();
	NoiseGenerator->SetFrequency(0.03f);
	NoiseGenerator->SetNoiseType(FastNoise::Perlin);
	NoiseGenerator->SetFractalType(FastNoise::FractalType::FBM);
}

void AMCChunkManager::GenerateInitialChunks()
{
	Chunks.Empty(DrawDistanceAroundPlayer * 2 * 2);

	for (int32 x = -DrawDistanceAroundPlayer; x < DrawDistanceAroundPlayer; x++)
	{
		for (int32 y = -DrawDistanceAroundPlayer; y < DrawDistanceAroundPlayer; y++)
		{
			auto CreatedChunk = GetWorld()->SpawnActor<AMCChunkBase>(
				ChunksToSpawn,
				FVector(static_cast<float>(x) * NewChunkSize.X * 100.f, static_cast<float>(y) * NewChunkSize.Y * 100.f, 0.f),
				FRotator::ZeroRotator);
			CreatedChunk->Tags.Add(DefaultChunkTag);
			CreatedChunk->MaterialInterface = ChunkMaterialInterface;
			CreatedChunk->Initialise(this, NoiseGenerator, NewChunkSize, FIntVector2(x, y));
			Chunks.Add(FIntVector2(x, y), CreatedChunk);
		}
	}
}

AMCChunkBase* AMCChunkManager::GetAdjacentChunk(AMCChunkBase* CurrentChunk, EMCDirection Direction)
{
	check(CurrentChunk);

	FIntVector2 ChunkPolling;
	switch (Direction)
	{
	case EMCDirection::Forward:
		ChunkPolling = FIntVector2(CurrentChunk->GetChunkLocation() + FIntVector2(0, 1));
		break;

	case EMCDirection::Back:
		ChunkPolling = FIntVector2(CurrentChunk->GetChunkLocation() + FIntVector2(0, -1));
		break;

	case EMCDirection::Left:
		ChunkPolling = FIntVector2(CurrentChunk->GetChunkLocation() + FIntVector2(-1, 0));
		break;

	case EMCDirection::Right:
		ChunkPolling = FIntVector2(CurrentChunk->GetChunkLocation() + FIntVector2(1, 0));
		break;

	default:
	case EMCDirection::Up:
	case EMCDirection::Down:
		UE_LOG(LogTemp, Error, TEXT("[ChunkWorld::GetAdjacentChunk]: Unsupported chunk adjaceny arguemnt."));
		return nullptr;
	}

	return Chunks.Contains(ChunkPolling) ? Chunks[ChunkPolling] : nullptr;
}
