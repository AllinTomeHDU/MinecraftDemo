// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "MotionMatchingAls/3C/Character/MMAlsCharacter.h"
#include "DemoCharacter.generated.h"

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

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnIsNoSeeChanged(bool bIsOwnerNoSee);

	UFUNCTION(BlueprintCallable)
	void OnHitActionComplete();

private:
	void HitBlock(const FHitResult& HitResult);

	UFUNCTION(Server, Reliable)
	void Server_HitBlock(const FHitResult HitResult);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HitBlock(const FHitResult HitResult);

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

	UPROPERTY(EditAnywhere, Category = "MC|Settings")
	float HitDistance = 400.f;
};
