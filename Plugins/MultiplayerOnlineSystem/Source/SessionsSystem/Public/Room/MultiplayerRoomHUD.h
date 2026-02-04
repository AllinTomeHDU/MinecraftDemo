// Copyright: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MultiplayerRoomHUD.generated.h"

class UMultiplayerRoomWidgetBase;

/**
 * 
 */
UCLASS()
class SESSIONSSYSTEM_API AMultiplayerRoomHUD : public AHUD
{
	GENERATED_BODY()
	
public:

protected:
	virtual void BeginPlay() override;

	void CreateRoomWidget();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SessionsSystem", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMultiplayerRoomWidgetBase> RoomWidgetClass;

	UPROPERTY()
	UMultiplayerRoomWidgetBase* RoomWidget;
};
