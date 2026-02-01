// Developer: Jichao Luo


#include "MCCharacterBase.h"


AMCCharacterBase::AMCCharacterBase()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}

void AMCCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

