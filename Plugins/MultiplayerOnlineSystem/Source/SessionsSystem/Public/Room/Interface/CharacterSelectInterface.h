// Copyright: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterSelectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterSelectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SESSIONSSYSTEM_API ICharacterSelectInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsSelected();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetSelected(const bool bValue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FString GetSelectedPlayerName() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FString GetSelecteddPlayerAccount() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetSelectedPlayer(const FString& PlayerAccount, const FString& PlayerName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetHightlight(const bool bValue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FString GetCharacterName() const;
};
