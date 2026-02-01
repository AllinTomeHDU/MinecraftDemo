// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "MCPluginTypes.generated.h"


UENUM(BlueprintType)
enum class EMCDirection : uint8
{
	Forward, Right, Back, Left, Up, Down
};

UENUM(BlueprintType)
enum class EMCBlock : uint8
{
	Null,
	Air, 
	Stone, 
	Dirt, 
	Grass, 
	Planks, 
	Inventory
};

// 存储模型数据
USTRUCT()
struct FMCChunkMeshData
{
	GENERATED_BODY();

	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector> Normals;
	TArray<FColor> Colors;
	TArray<FVector2D> UV0;

	void Clear()
	{
		Vertices.Empty();
		Triangles.Empty();
		Normals.Empty();
		Colors.Empty();
		UV0.Empty();
	}
};
