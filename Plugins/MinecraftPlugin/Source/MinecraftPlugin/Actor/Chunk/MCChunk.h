// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "MCChunkBase.h"
#include "MCChunk.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API AMCChunk : public AMCChunkBase
{
	GENERATED_BODY()

	// 用于在检索面（faces）时的掩码（Mask）
	struct FMask
	{
		EMCBlock Block;
		int Normal;
	};

	FMCChunkMeshData MeshData;
	TArray<EMCBlock> Blocks;

	int VertexCount = 0;
	
public:
	AMCChunk();

	void ClearMeshData();

	// 根据坐标获取 Block类型
	virtual EMCBlock GetBlock(const FIntVector& Coord) const override;

	virtual void ModifyVoxel(const FIntVector Position, const EMCBlock NewBlock) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnInitialiseComplete() override;

private:
	void ModifyVoxelData(const FIntVector Position, EMCBlock NewBlock);

	// 用于处理 “坐标越界（Coordinate Overflow）” 时该往哪个方向查找相邻区块（Chunk）
	EMCDirection GetCoordinateOverflowDirection(const FIntVector& Coord, FIntVector& OutCoord) const;

	// 根据 方块类型（Block） 和 当前面的法线方向（Normal）返回该面在 Texture2DArray 中的索引值
	int32 GetTextureIndex(EMCBlock Block, FVector Normal) const;

	// 根据给定的方块面（Mask）信息，生成一个四边形（Quad）面并写入网格数据（MeshData）中
	void CreateQuad(FMask Mask, FIntVector AxisMask, const int Width, const int Height, 
					FIntVector V1, FIntVector V2, FIntVector V3, FIntVector V4);

	// 判断这两个“方块面”在视觉与逻辑上是否可以被合并或视为相同面
	bool CompareMask(const FMask M1, const FMask M2) const;

	/**
	 * 算法思路：
	 *   1. 根据 Perlin/Simplex 噪声生成一个“地形高度（Height）”
	 *   2. 然后在该高度之上是空气、在顶面放草皮、往下放泥土和石头，最终形成一片自然的地形。
	 */
	void GenerateBlocks();
	
	/**
	* 算法思路：
	*   1. 对每个主轴（X、Y、Z）把体素区块看作一系列平行切片（slice），对每个切片建立一个 2D Mask 表示每个格子是否需要渲染面以及面属于哪个方块、朝向哪边。
	*   2. 对这个 Mask 做 贪婪合并（Greedy Meshing）：在 2D 网格上尽可能把相邻且相同 Mask 的单元合并成更大的矩形（quad），从而大幅减少生成的三角形数量。
	*/
	void GenerateMesh();

	/**
	* 把 GenerateMesh() 中生成的所有几何数据（顶点、三角形、法线、UV、颜色等）
	* 上传到 UProceduralMeshComponent（Mesh）上，让引擎创建可渲染的网格段（Section）
	*/
	void ApplyMesh() const;

	// 将三维坐标转换为一维索引
	FORCEINLINE int32 GetBlockIndex(const int32 X, const int32 Y, const int32 Z) const
	{
		return Z * ChunkSize.X * ChunkSize.Y + Y * ChunkSize.X + X;
	}
	FORCEINLINE int32 GetBlockIndex(const FVector& Position) const 
	{ 
		return GetBlockIndex(static_cast<int>(Position.X), static_cast<int>(Position.Y), static_cast<int>(Position.Z));
	}
	FORCEINLINE int32 GetBlockIndex(const FIntVector& Position) const
	{
		return GetBlockIndex(Position.X, Position.Y, Position.Z);
	}

	// 判断给定的方块坐标是否在当前区块（Chunk）的合法范围内
	FORCEINLINE bool IsInBounds(int32 X, int32 Y, int32 Z) const
	{
		return X >= 0 && Y >= 0 && Z >= 0 && X < ChunkSize.X && Y < ChunkSize.Y && Z < ChunkSize.Z;
	}
	FORCEINLINE bool IsInBounds(FIntVector Coords) const 
	{ 
		return IsInBounds(Coords.X, Coords.Y, Coords.Z); 
	}

};
