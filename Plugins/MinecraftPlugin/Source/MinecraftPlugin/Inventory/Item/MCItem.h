// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MCItem.generated.h"

class UMCItemDatabase;

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API UMCItem : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	FString AssociatedItemID;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentStack;

	UPROPERTY(VisibleAnywhere)
	int32 MaxStack;

public:
	void Initialize(const FString& InAssociatedItemID, const int32 CurrentStackSize = 1);

	FORCEINLINE FString GetAssociatedItemID() const { return AssociatedItemID; }
	FORCEINLINE int32 GetCurrentStack() const { return CurrentStack; }
	
	bool SetCurrentStackChecked(int32 NewStack, const UMCItemDatabase* ItemInfoDB);
	bool SetCurrentStackUnChecked(int32 NewStack);
};
