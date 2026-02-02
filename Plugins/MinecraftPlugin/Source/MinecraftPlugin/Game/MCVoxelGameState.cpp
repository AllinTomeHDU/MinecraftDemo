// Developer: Jichao Luo


#include "MCVoxelGameState.h"


UMCInventoryDatabase& AMCVoxelGameState::GetInventoryDatabase()
{
	if (!IsValid(MainInventoryDatabase))
	{
		MainInventoryDatabase = NewObject<UMCInventoryDatabase>();
		MainInventoryDatabase->Initialize();
	}
	return *MainInventoryDatabase;
}
