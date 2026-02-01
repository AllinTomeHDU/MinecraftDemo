// Developer: Jichao Luo


#include "MCHUD.h"
#include "Blueprint/UserWidget.h"


AMCHUD::AMCHUD()
{
}

void AMCHUD::BeginPlay()
{
	Super::BeginPlay();

	checkf(CrosshairWidgetClass, TEXT("AMCHUD::CrosshairWidgetClass is nullptr"));
	CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
	if (IsValid(CrosshairWidget))
	{
		CrosshairWidget->AddToViewport();
	}
}
