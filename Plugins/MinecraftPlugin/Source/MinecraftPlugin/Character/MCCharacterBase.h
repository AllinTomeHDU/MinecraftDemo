// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MCCharacterBase.generated.h"

UCLASS(Abstract)
class MINECRAFTPLUGIN_API AMCCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AMCCharacterBase();

protected:
	virtual void BeginPlay() override;


};
