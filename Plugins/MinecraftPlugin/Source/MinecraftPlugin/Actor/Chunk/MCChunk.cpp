// Developer: Jichao Luo


#include "MCChunk.h"
#include "MCChunkManager.h"
#include "ProceduralMeshComponent.h"
#include "FastNoise.h"


AMCChunk::AMCChunk()
{
}

void AMCChunk::BeginPlay()
{
	Super::BeginPlay();
}

void AMCChunk::OnInitialiseComplete()
{
	Blocks.SetNum(ChunkSize.X * ChunkSize.Y * ChunkSize.Z);

	GenerateBlocks();
	GenerateMesh();
	ApplyMesh();
}

void AMCChunk::ClearMeshData()
{
	VertexCount = 0;
	MeshData.Clear();
}

void AMCChunk::ModifyVoxel(const FIntVector Position, const EMCBlock NewBlock)
{
	if (!IsInBounds(Position))
	{
		FIntVector OutCoord;
		AMCChunkBase* AdjacentChunk = GetChunkManager()->GetAdjacentChunk(this, GetCoordinateOverflowDirection(Position, OutCoord));
		if (AdjacentChunk)
		{
			AdjacentChunk->ModifyVoxel(OutCoord, NewBlock);
		}
		return;
	}
	ModifyVoxelData(Position, NewBlock);
	ClearMeshData();
	GenerateMesh();
	ApplyMesh();
}

EMCBlock AMCChunk::GetBlock(const FIntVector& Coord) const
{
	if (Coord.X >= ChunkSize.X || Coord.Y >= ChunkSize.Y || Coord.Z >= ChunkSize.Z ||
		Coord.X < 0 || Coord.Y < 0 || Coord.Z < 0) return EMCBlock::Air;

	return Blocks[GetBlockIndex(Coord)];
}

void AMCChunk::ModifyVoxelData(const FIntVector Position, EMCBlock NewBlock)
{
	Blocks[GetBlockIndex(Position)] = NewBlock;
}

EMCDirection AMCChunk::GetCoordinateOverflowDirection(const FIntVector& Coord, FIntVector& OutCoord) const
{
	OutCoord = FIntVector(Coord);

	if (Coord.X > this->GetChunkSize().X)
	{
		OutCoord.X = 0;
		return EMCDirection::Right;
	}
	else if (Coord.X < 0)
	{
		OutCoord.X = this->GetChunkSize().X - 1;
		return EMCDirection::Left;
	}
	else if (Coord.Y > this->GetChunkSize().Y)
	{
		OutCoord.Y = 0;
		return EMCDirection::Forward;
	}
	else if (Coord.Y < 0)
	{
		OutCoord.Y = this->GetChunkSize().Y - 1;
		return EMCDirection::Back;
	}
	return EMCDirection::Down;
}

int32 AMCChunk::GetTextureIndex(EMCBlock Block, FVector Normal) const
{
	switch (Block)
	{
	case EMCBlock::Grass:
		if (Normal == FVector::UpVector) // TODO: Fix floating point precision.
			return 0;
		if (Normal == FVector::DownVector)
			return 2;
		return 1;

	case EMCBlock::Dirt:
		return 2;

	case EMCBlock::Stone:
		return 3;

	case EMCBlock::Inventory:
		if (Normal == FVector::UpVector)
			return 4;
		if (Normal == FVector::DownVector)
			return 7;
		if (Normal == FVector::ForwardVector)
			return 6;
		return 5;

	case EMCBlock::Planks:
		return 7;

	default:
#if UE_BUILD_DEVELOPMENT
		UE_LOG(LogTemp, Warning, TEXT("[AChunk::GetTextureIndex]: Unset value for texture index. Returning 0."));
#endif
		return 0;
	}
}

void AMCChunk::CreateQuad(FMask Mask, FIntVector AxisMask, const int Width, const int Height, 
						  FIntVector V1, FIntVector V2, FIntVector V3, FIntVector V4)
{
	// 计算法线方向
	const auto Normal = FVector(AxisMask * Mask.Normal);

	// 写入顶点位置：这4个点组成一个四边形，每个点乘以 100 表示一个体素大小为 100cm
	MeshData.Vertices.Add(FVector(V1) * 100);
	MeshData.Vertices.Add(FVector(V2) * 100);
	MeshData.Vertices.Add(FVector(V3) * 100);
	MeshData.Vertices.Add(FVector(V4) * 100);

	// 写入三角形索引：无论法线方向是 +X 还是 -X，都能生成正确朝向的三角面
	MeshData.Triangles.Add(VertexCount);
	MeshData.Triangles.Add(VertexCount + 2 + Mask.Normal);
	MeshData.Triangles.Add(VertexCount + 2 - Mask.Normal);
	MeshData.Triangles.Add(VertexCount + 3);
	MeshData.Triangles.Add(VertexCount + 1 - Mask.Normal);
	MeshData.Triangles.Add(VertexCount + 1 + Mask.Normal);

	// 设置UV坐标：根据法线朝向（X面 or YZ面）调整UV的映射方向，Width、Height 表示当前合并的方块数
	if (Normal.X == 1 || Normal.X == -1)
	{
		MeshData.UV0.Append({
			FVector2D(Width, Height),
			FVector2D(0, Height),
			FVector2D(Width, 0),
			FVector2D(0, 0)});
	}
	else
	{
		MeshData.UV0.Append({
			FVector2D(Height, Width),
			FVector2D(Height, 0),
			FVector2D(0, Width),
			FVector2D(0, 0)});
	}

	// 添加顶点颜色：轻量级纹理索引传递方式
	auto Color = FColor(0, 0, 0, GetTextureIndex(Mask.Block, Normal));
	MeshData.Colors.Append({ Color, Color, Color, Color });

	// 添加法线：给每个顶点都写入相同的法线，用于光照、阴影计算，每个四边形面在渲染时具有一致的平面法线
	MeshData.Normals.Add(Normal);
	MeshData.Normals.Add(Normal);
	MeshData.Normals.Add(Normal);
	MeshData.Normals.Add(Normal);

	// 更新顶点计数
	VertexCount += 4;
}

bool AMCChunk::CompareMask(const FMask M1, const FMask M2) const
{
	return M1.Block == M2.Block && M1.Normal == M2.Normal;
}

void AMCChunk::GenerateBlocks()
{
	const auto Location = GetActorLocation();

	for (auto x = 0.f; x < static_cast<float>(ChunkSize.X); x += 1.f)
	{
		for (auto y = 0.f; y < static_cast<float>(ChunkSize.Y); y += 1.f)
		{
			// 计算噪声坐标，把世界位置转为噪声坐标系
			const auto XPos = (x * 100.f + static_cast<float>(Location.X)) / 100.f;
			const auto YPos = (y * 100.f + static_cast<float>(Location.Y)) / 100.f;

			// 得到一个 [0, ChunkSize.Z] 范围的整数值，表示该 (x, y) 位置上的地表高度
			const auto Height = FMath::Clamp(
				FMath::RoundToInt((NoiseGenerator->GetNoise(XPos, YPos) + 1.f) * static_cast<float>(ChunkSize.Z) / 2.f), 
				0,
				static_cast<float>(ChunkSize.Z)
			);

			// TODO: Consider rewriting to use int in the first instance.

			for (auto z = 0.f; z < static_cast<float>(ChunkSize.Z); z += 1.f)
			{
				if (z > Height)  // Air
				{
					Blocks[GetBlockIndex(x, y, z)] = EMCBlock::Air;
				}
				else if (z == Height)  // Grass
				{
					Blocks[GetBlockIndex(x, y, z)] = EMCBlock::Grass;
				}
				else if (z < Height && z > Height - 4)  // Dirt
				{
					Blocks[GetBlockIndex(x, y, z)] = EMCBlock::Dirt;
				}
				else  // Stone
				{
					Blocks[GetBlockIndex(x, y, z)] = EMCBlock::Stone;
				}
			}
		}
	}
}

void AMCChunk::GenerateMesh()
{
	// 遍历三个主轴：0=X, 1=Y, 2=Z
	// 思路：对每个主轴，按“切片”去做 2D 贪婪合并（Greedy Meshing）
	for (auto Axis = 0; Axis < 3; Axis++)
	{
		// 两个垂直于 Axis 的轴索引（用于构建 2D 面）
		const auto Axis1 = (Axis + 1) % 3;
		const auto Axis2 = (Axis + 2) % 3;

		// 每个轴在当前 chunk 的长度（上限）
		const auto MainAxisLimit = ChunkSize[Axis];
		const auto Axis1Limit = ChunkSize[Axis1];
		const auto Axis2Limit = ChunkSize[Axis2];

		// 记录在 Axis1 和 Axis2 方向上的扩展量（合并宽/高）
		auto DeltaAxis1 = FIntVector::ZeroValue;
		auto DeltaAxis2 = FIntVector::ZeroValue;

		// 表示正在遍历的三维坐标（在循环中作为局部坐标使用）
		auto ChunkIteration = FIntVector::ZeroValue;

		// 掩码用于表示当前是在检查哪个面方向（例如：AxisMask = (1,0,0) 意味着 X 面）
		auto AxisMask = FIntVector::ZeroValue;
		AxisMask[Axis] = 1;

		// Mask 是一个 2D 数组（用一维 TArray 来表示），存放每个单元格的 FMask 信息
		// 大小 = Axis1Limit * Axis2Limit（因为每个切片是 Axis1 x Axis2 的平面）
		TArray<FMask> Mask;
		Mask.SetNum(Axis1Limit * Axis2Limit);

		// 对每个切片进行处理。ChunkIteration[Axis] 从 -1 到 MainAxisLimit-1，
		// 之所以从 -1 开始，是为了能比较“当前块和其 +Axis 方向的邻块”，
		// 这样第一个切片会比较 -1 -> 0（用于边界生成），最后一个切片会比较 MainAxisLimit-1 -> MainAxisLimit（用于边界）
		for (ChunkIteration[Axis] = -1; ChunkIteration[Axis] < MainAxisLimit; )
		{
			auto N = 0; // Mask 索引（遍历 2D 面时的线性索引）
			for (ChunkIteration[Axis2] = 0; ChunkIteration[Axis2] < Axis2Limit; ChunkIteration[Axis2]++)
			{
				for (ChunkIteration[Axis1] = 0; ChunkIteration[Axis1] < Axis1Limit; ChunkIteration[Axis1]++)
				{
					const auto CurrentBlock = GetBlock(ChunkIteration);
					const auto ComparEMCBlock = GetBlock(ChunkIteration + AxisMask); // Adjacent block.

					// 判断是否为不透明（占据体素的块） — 这里示例用 != Air 判断
					const auto IsCurrentBlockOpaque = CurrentBlock != EMCBlock::Air;
					const auto IsComparEMCBlockOpaque = ComparEMCBlock != EMCBlock::Air;

					if (IsCurrentBlockOpaque == IsComparEMCBlockOpaque)
					{
						// 如果两边都是不透明或两边都透明，则该位置无可见面（mask.Normal = 0）
						Mask[N++] = FMask{ EMCBlock::Null, 0 };
					}
					else if (IsCurrentBlockOpaque)
					{
						// 如果当前块不透明而比较块透明 -> 说明我们需要渲染当前块面，法线朝向 +Axis（用 1 表示）
						Mask[N++] = FMask{ CurrentBlock, 1 }; 
					}
					else
					{
						// 否则比较块不透明而当前块透明 -> 说明渲染比较块的面，法线朝向 -Axis（用 -1 表示）
						Mask[N++] = FMask{ ComparEMCBlock, -1 };
					}
				}
			}

			// 移动到下一层（切片沿主轴方向前进一步），重置 mask 索引，准备合并算法遍历 Mask
			ChunkIteration[Axis]++;
			N = 0;

			// ---------- 从 Mask 生成合并后的四边形（Quad） ----------
			// 对 mask 的每个单元执行贪婪合并（先横向扩展 Width，再竖向扩展 Height）
			for (auto j = 0; j < Axis2Limit; j++)
			{
				for (auto i = 0; i < Axis1Limit; )
				{
					// 如果当前 mask 单元为 0（无面），跳过
					if (Mask[N].Normal == 0)
					{
						i++;
						N++;
						continue;
					}

					// 当前可合并目标的 mask，将 ChunkIteration 的 Axis1/Axis2 分量设为当前 mask 单元的位置（i,j）
					const auto CurrentMask = Mask[N];
					ChunkIteration[Axis1] = i;
					ChunkIteration[Axis2] = j;

					int Width;
					for (Width = 1; i + Width < Axis1Limit && CompareMask(Mask[N + Width], CurrentMask); Width++)
					{
						// 空体，计算宽度（沿 Axis1 方向尽可能扩展）
					}

					// 计算高度（沿 Axis2 方向扩展），必须保证每一行的每个单元都和 CurrentMask 相同
					bool bIsDone = false;
					int Height;
					for (Height = 1; j + Height < Axis2Limit; Height++)
					{
						for (auto k = 0; k < Width; k++)
						{
							if (CompareMask(Mask[N + k + Height * Axis1Limit], CurrentMask))
								continue;

							bIsDone = true;
							break;
						}

						if (bIsDone) break;
					}

					// 记录扩展量（为后面生成四边形计算顶点）
					DeltaAxis1[Axis1] = Width;
					DeltaAxis2[Axis2] = Height;

					// 把这个合并的矩形生成为实际几何（顶点/三角形/UV/颜色/法线）
					// CreateQuad 接受四个角的局部坐标（ChunkIteration, ChunkIteration + DeltaAxis1, ...）
					CreateQuad(CurrentMask, AxisMask, Width, Height,
						ChunkIteration,
						ChunkIteration + DeltaAxis1,
						ChunkIteration + DeltaAxis2,
						ChunkIteration + DeltaAxis1 + DeltaAxis2);

					// 清空临时扩展向量
					DeltaAxis1 = FIntVector::ZeroValue;
					DeltaAxis2 = FIntVector::ZeroValue;

					// 将 Mask 中被合并的所有单元标记为已处理（设为 Null/0）
					for (auto l = 0; l < Height; l++)
					{
						for (auto k = 0; k < Width; k++)
						{
							Mask[N + k + l * Axis1Limit] = FMask{ EMCBlock::Null, 0 };
						}
					}

					// 跳过已经合并过的宽度单元
					i += Width;
					N += Width;
				}
			}
		}
	}
}

void AMCChunk::ApplyMesh() const
{
	Mesh->SetMaterial(0, MaterialInterface);
	Mesh->CreateMeshSection(
		0, 
		MeshData.Vertices, 
		MeshData.Triangles, 
		MeshData.Normals, 
		MeshData.UV0, 
		MeshData.Colors, 
		TArray<FProcMeshTangent>(), 
		true
	);
}
