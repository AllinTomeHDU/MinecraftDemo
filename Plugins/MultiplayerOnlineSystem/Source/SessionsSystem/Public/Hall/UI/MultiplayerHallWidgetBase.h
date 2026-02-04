// Copyright: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/MultiplayerSessionsBPLibrary.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Definition/BlueprintOnlineHelperType.h"
#include "MultiplayerHallWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class SESSIONSSYSTEM_API UMultiplayerHallWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

public:


protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void UpdateHallWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateHallWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCreateSession();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFindSessions(const TMap<FString, FSessionsSearchResult>& SearchResults);

	UFUNCTION(BlueprintImplementableEvent)
	void OnJoinSession(const ESessionsJoinResult JoinResult);

private:
	void OnCreateSessionComplete(bool bWasSuccessful);
	void OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSeccessful);
	void OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type ResultType);

private:
	FTimerHandle UpdateHallHandle;

	UPROPERTY(EditDefaultsOnly, Category = "SessionsSystem", meta = (AllowPrivateAccess = "true"))
	float UpdateHallInterval = 2.f;
};
