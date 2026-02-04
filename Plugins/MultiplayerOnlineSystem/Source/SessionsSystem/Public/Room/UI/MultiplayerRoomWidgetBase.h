// Copyright: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerRoomWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class SESSIONSSYSTEM_API UMultiplayerRoomWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

public:


protected:
	virtual void NativeConstruct() override;

	virtual void UpdateRoomWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateRoomWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDestroySession();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartSession();

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndSession();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateSession();

private:
	void OnDestroySessionComplete(bool bWasSuccessful);
	void OnStartSessionComplete(bool bWasSuccessful);
	void OnEndSessionComplete(bool bWasSuccessful);
	void OnUpdateSessionComplete(bool bWasSuccessful);

private:
	FTimerHandle UpdateRoomHandle;

	UPROPERTY(EditDefaultsOnly, Category = "SessionsSystem", meta = (AllowPrivateAccess = "true"))
	float UpdateRoomInterval = 0.5f;
};
