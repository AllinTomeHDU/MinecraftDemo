// Copyright: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerHallHUD.generated.h"


/**
 * 
 */
UCLASS()
class SESSIONSSYSTEM_API AMultiplayerHallHUD : public AHUD
{
	GENERATED_BODY()

public:
	
protected:
	virtual void BeginPlay() override;

	void CreateMainPageWidget();
	void CreateHallWidget();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SessionsSystem", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> RegisterLoginWidgetClass;

	UPROPERTY()
	UUserWidget* RegisterLoginWidget;

#ifdef CLIENT_SYSTEM
	void OnRegisterLoginWidgetDestruct();
#endif

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SessionsSystem", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> MainPageWidgetClass;

	UPROPERTY()
	UUserWidget* MainPageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SessionsSystem", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HallWidgetClass;

	UPROPERTY()
	UUserWidget* HallWidget;
};
