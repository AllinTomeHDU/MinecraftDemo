// Developer: Jichao Luo


#include "MCChunkBase.h"
#include "ProceduralMeshComponent.h"
#include "FastNoise.h"


AMCChunkBase::AMCChunkBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	Mesh->SetCastShadow(false);
	SetRootComponent(Mesh);
}

void AMCChunkBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMCChunkBase::Initialise(AMCChunkManager* InChunkManager, FastNoise* InNoiseGen, const FIntVector InChunkSize, const FIntVector2 InChunkLocation)
{
	ChunkManager = InChunkManager;
	NoiseGenerator = InNoiseGen;
	ChunkSize = InChunkSize;
	ChunkLocation = InChunkLocation;

	OnInitialiseComplete();
}
