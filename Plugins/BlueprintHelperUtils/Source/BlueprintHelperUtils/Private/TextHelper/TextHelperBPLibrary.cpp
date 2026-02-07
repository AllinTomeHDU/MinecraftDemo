#include "TextHelper/TextHelperBPLibrary.h"


int32 UTextHelperBPLibrary::FormatNumberStringInput(const FString& InString, const int32& LastNumber, int32 Min, int32 Max)
{
	if (InString.IsEmpty())
	{
		return LastNumber;
	}

	if (InString.IsNumeric())
	{
		int32 Value = FCString::Atoi(*InString);
		if (Value < Min)
		{
			Value = Min;
		}
		else if (Value > Max)
		{
			Value = Max;
		}
		return Value;
	}
	return LastNumber;
}

FString UTextHelperBPLibrary::FormatStringLengthInput(const FString& InString, int32 MaxLength)
{
	return InString.Len() <= MaxLength ? InString : InString.Left(MaxLength);
}
