#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TextHelperBPLibrary.generated.h"


UCLASS()
class BLUEPRINTHELPERUTILS_API UTextHelperBPLibrary : public UObject
{
	GENERATED_BODY()

public:
	/*
	* 限制输入字符串为数字字符串，并限制范围在Min到Max之间，返回格式化后的数字
	*/
	UFUNCTION(BlueprintCallable, Category = "TextHelper")
	static int32 FormatNumberStringInput(const FString& InString, const int32& LastNumber, int32 Min, int32 Max);

	/*
	* 限制输入字符串的长度，返回格式化后的字符串
	*/
	UFUNCTION(BlueprintCallable, Category = "TextHelper")
	static FString FormatStringLengthInput(const FString& InString, int32 MaxLength = 25);

};