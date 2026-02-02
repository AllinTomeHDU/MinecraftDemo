// Developer: Jichao Luo


#include "MCVoxelHUD.h"
#include "MinecraftPlugin/UI/Widget/MCUserWidgetBase.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCPlayerHotbarWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCInventoryVisualizerWidget.h"


AMCVoxelHUD::AMCVoxelHUD()
{
}

void AMCVoxelHUD::BeginPlay()
{
	Super::BeginPlay();

	checkf(CrosshairWidgetClass, TEXT("AMCHUD::CrosshairWidgetClass is nullptr"));
	CrosshairWidget = CreateWidget<UMCUserWidgetBase>(GetWorld(), CrosshairWidgetClass);
	if (IsValid(CrosshairWidget))
	{
		CrosshairWidget->AddToViewport(-1);
	}

	checkf(PlayerHotbarWidgetClass, TEXT("AMCHUD::PlayerHotbarWidgetClass is nullptr"));
	PlayerHotbarWidget = CreateWidget<UMCPlayerHotbarWidget>(GetWorld(), PlayerHotbarWidgetClass);
	if (IsValid(PlayerHotbarWidget))
	{
		PlayerHotbarWidget->AddToViewport(0);
	}

	checkf(InventoryVisualizerWidgetClass, TEXT("AMCHUD::InventoryVisualizerWidgetClass is nullptr"));
	InventoryVisualizerWidget = CreateWidget<UMCInventoryVisualizerWidget>(GetWorld(), InventoryVisualizerWidgetClass);
	if (IsValid(InventoryVisualizerWidget))
	{
		InventoryVisualizerWidget->AddToViewport(1);
	}
}
