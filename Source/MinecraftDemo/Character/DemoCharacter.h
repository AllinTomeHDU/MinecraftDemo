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

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Head;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* LeftHandScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LeftHandObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RightHandScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RightHandObject;
	
};
