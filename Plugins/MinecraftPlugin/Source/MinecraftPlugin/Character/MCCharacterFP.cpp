// Developer: Jichao Luo


#include "MCCharacterFP.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"


AMCCharacterFP::AMCCharacterFP()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->SetRelativeLocation(FVector(0.f, 25.f, 170.f));
	CameraBoom->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	CameraBoom->TargetArmLength = 1.f;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 50.f;
	CameraBoom->CameraRotationLagSpeed = 50.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = true;

	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head && Expression"));
	Head->SetupAttachment(GetMesh(), FName("head"));
	Head->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));

	LeftHandScene = CreateDefaultSubobject<USceneComponent>(TEXT("LeftHandScene"));
	LeftHandScene->SetupAttachment(CameraBoom);
	LeftHandScene->SetRelativeLocation(FVector(45.f, -45.f, -20.f));
	LeftHandScene->SetRelativeRotation(FRotator(30.f, 185.f, 90.f));
	LeftHandScene->SetRelativeScale3D(FVector(0.45f, 0.45f, 0.45f));

	LeftHandObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftHandObject"));
	LeftHandObject->SetupAttachment(LeftHandScene);

	RightHandScene = CreateDefaultSubobject<USceneComponent>(TEXT("RightHandScene"));
	RightHandScene->SetupAttachment(CameraBoom);
	RightHandScene->SetRelativeLocation(FVector(45.f, 45.f, -20.f));
	RightHandScene->SetRelativeRotation(FRotator(30.f, 175.f, 90.f));
	RightHandScene->SetRelativeScale3D(FVector(0.45f, 0.45f, 0.45f));

	RightHandObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightHandObject"));
	RightHandObject->SetupAttachment(RightHandScene);

}
