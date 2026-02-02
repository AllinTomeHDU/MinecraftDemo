// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MCVoxelHUD.generated.h"

class UMCUserWidgetBase;
class UMCPlayerHotbarWidget;

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API AMCVoxelHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AMCVoxelHUD();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMCUserWidgetBase> CrosshairWidgetClass;

	UPROPERTY()
	UMCUserWidgetBase* CrosshairWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMCPlayerHotbarWidget> PlayerHotbarWidgetClass;

	UPROPERTY()
	UMCPlayerHotbarWidget* PlayerHotbarWidget;
	
};
