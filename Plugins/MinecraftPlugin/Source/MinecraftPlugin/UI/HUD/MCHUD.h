// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MCHUD.generated.h"

 

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API AMCHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AMCHUD();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY()
	UUserWidget* CrosshairWidget;
	
};
