// Copyright: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MultiplayerRoomController.generated.h"

class UInputMappingContext;
class UInputAction;
class UAnimInstance;

/**
 * 
 */
UCLASS()
class SESSIONSSYSTEM_API AMultiplayerRoomController : public APlayerController
{
	GENERATED_BODY()

public:
	AMultiplayerRoomController();

	void Logout();
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel) override;

	void OnClickAction();

	UFUNCTION(Server, Reliable)
	void Server_SelectCharacter(ACharacter* TargetCharacter, bool bIsSelected);

	UFUNCTION(Server, Reliable)
	void Server_SetSelectedPlayer(ACharacter* TargetCharacter, const FString& PlayerAccout, const FString& PlayerName);

	void UpdateSessionPlayers();

private:
	UPROPERTY(Replicated)
	ACharacter* SelectedCharacter;

	UPROPERTY(EditAnywhere, Category = "Multiplayer Room")
	TSubclassOf<UAnimInstance> SelectedCharacterAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* IMC_Default;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Click;

	AActor* LastCursorActor;
	AActor* ThisCursorActor;

	UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> TravelTransitionWidgetClass;

	bool bPendingKillOnTravel = false;	// 额外安全保护，避免SeamlessTravel结束时还Tick
};
