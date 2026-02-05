// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "MotionMatchingAls/3C/Character/MMAlsCharacter.h"
#include "MinecraftPlugin/Library/Definition/MCPluginTypes.h"
#include "DemoCharacter.generated.h"

class AMCChunkBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitBlockActionSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlaceBlockActionSignature);

/**
 * 
 */
UCLASS()
class MINECRAFTDEMO_API ADemoCharacter : public AMMAlsCharacter
{
	GENERATED_BODY()
	
public:
	ADemoCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void UpdateMeshesRenderingMode();

	void SetHandsObjectVisibility(const bool bVisible);
	void SetBodyOwnerNoSee(const bool bIsNoSee);

	void ToggleLocomotionMode();

	UPROPERTY(BlueprintAssignable)
	FOnHitBlockActionSignature OnHitBlockActionDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnPlaceBlockActionSignature OnPlaceBlockActionDelegate;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void Server_ToggleLocomotionMode(const bool bMotionMatching);

	UFUNCTION()
	void OnIsNoSeeChanged(bool bIsOwnerNoSee);

	UFUNCTION(BlueprintCallable)
	void OnHitActionComplete();

	void HitBlock(const FVector& TraceStart, const FVector& TraceEnd);

	UFUNCTION(Server, Reliable)
	void Server_HitBlock(const FVector TraceStart, const FVector TraceEnd);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HitBlock(const FVector TraceStart, const FVector TraceEnd);

	UFUNCTION(BlueprintCallable)
	void OnPlaceActionComplete();

	void PlaceBlock(const FVector& TraceStart, const FVector& TraceEnd);

	UFUNCTION(Server, Reliable)
	void Server_PlaceBlock(const FVector TraceStart, const FVector TraceEnd);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaceBlock(const FVector TraceStart, const FVector TraceEnd);

	void PlaceBlock(AMCChunkBase* Chunk, const FVector& WorldPos, const FVector& HitNormal, EMCBlock Block);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Head;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* LeftHandScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LeftHandObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RightHandScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RightHandObject;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "MC|Settings", meta = (AllowPrivateAccess = "true"))
	bool bUseMotionMatching{ false };

	UPROPERTY(EditAnywhere, Category = "MC|Settings")
	bool bForceHideHandsObject{ false };

	UPROPERTY(EditAnywhere, Category = "MC|Settings")
	float HitDistance = 400.f;
};
