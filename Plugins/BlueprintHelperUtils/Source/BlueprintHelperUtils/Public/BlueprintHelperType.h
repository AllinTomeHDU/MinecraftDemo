#pragma once

#include "CoreMinimal.h"
#include "BlueprintHelperType.generated.h"


UENUM(BlueprintType)
enum class EBP_OperationResult : uint8
{
	Success     UMETA(DisplayName = "Success"),
	Warning     UMETA(DisplayName = "Warning"),
	Error       UMETA(DisplayName = "Error")
};