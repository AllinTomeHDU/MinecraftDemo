// Developer: Jichao Luo


#include "MCInventoryVisualizerWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCInventorySlotWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCInventoryItemWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCInventoryGridWidget.h"
#include "MinecraftPlugin/Inventory/MCInventoryInterface.h"
#include "MinecraftPlugin/Inventory/MCPlayerInventory.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"


void UMCInventoryVisualizerWidget::ChangePlayerInputsToWorld()
{
	if (auto PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}
}

void UMCInventoryVisualizerWidget::ToggleInventoryState(UMCPlayerInventory* InPlayerInventory, bool& OutIsMenuDisplayed)
{
	switch (VisualiserState)
	{
		case EMCInventoryVisualiserState::Hidden:
		{
			InitPlayerInventoryWidget(InPlayerInventory);
			CurrentPlayerInventory = InPlayerInventory;
			OutIsMenuDisplayed = true;
			VisualiserState = EMCInventoryVisualiserState::ShowingPlayer;
			break;
		}
		case EMCInventoryVisualiserState::ShowingPlayer:
		{
			HidePlayerInventory();
			CurrentPlayerInventory = nullptr;
			ChangePlayerInputsToWorld();
			OutIsMenuDisplayed = false;
			VisualiserState = EMCInventoryVisualiserState::Hidden;
			break;
		}
		case EMCInventoryVisualiserState::ShowingBoth:
		{
			HideSecondaryInventory();
			CurrentOtherInventory = nullptr;
			OutIsMenuDisplayed = InventoryGrid_Player->GetVisibility() == ESlateVisibility::Visible;
			VisualiserState = OutIsMenuDisplayed ? EMCInventoryVisualiserState::ShowingPlayer
												 : EMCInventoryVisualiserState::Hidden;
		}
	}
}

void UMCInventoryVisualizerWidget::OnSlotButtonClicked(UMCInventorySlotWidget* InSlotWidget)
{
	if (CurrentHeldItem && InSlotWidget->IsHoldingWidget())
	{
		// 从库中取出资产
	}
	else if (!CurrentHeldItem || !InSlotWidget->IsHoldingWidget())
	{
		// 将资产存入库中
	}
}

void UMCInventoryVisualizerWidget::PlayerGridAddWidget(UUserWidget* InWidget)
{
	InventoryGrid_Player->GetWrapBox_InventorySlot()->AddChild(InWidget);
}

void UMCInventoryVisualizerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UMCInventoryVisualizerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

//FReply UMCInventoryVisualizerWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
//{
//	if (State == Hidden) return FReply::Unhandled();
//
//	// TODO: Explore getting the keys to check for from the input here specifically. 
//	// It seems possible for the player input but not here? Who designed this.
//	if (InKeyEvent.GetKey() == EKeys::I || InKeyEvent.GetKey() == EKeys::E)
//	{
//		if (!GetWorld()->GetFirstPlayerController()) return FReply::Unhandled();
//		if (CurrentHeldItem != nullptr) return FReply::Unhandled();
//
//		Hide();
//		return FReply::Handled();
//	}
//	return FReply::Unhandled();
//}

//void UMCInventoryVisualizerWidget::Hide()
//{
//	switch (State)
//	{
//	case Hidden:
//		break;
//
//	case ShowingBoth: // TODO: Identical to TogglePlayerInventory, ToggleBothInventories
//		HideSecondaryInventory();
//		CurrentOtherInventory = nullptr;
//
//	case ShowingPlayer:
//		HidePlayerInventory();
//		CurrentOtherInventory = nullptr;
//		ChangePlayerInputsToWorld();
//		State = Hidden;
//		break;
//	}
//}

void UMCInventoryVisualizerWidget::HideSecondaryInventory()
{
	InventoryGrid_Other->GetWrapBox_InventorySlot()->ClearChildren();
	Overlay_OtherInventory->SetVisibility(ESlateVisibility::Hidden);
}

void UMCInventoryVisualizerWidget::HidePlayerInventory()
{
	InventoryGrid_Player->GetWrapBox_InventorySlot()->ClearChildren();
	Overlay_PlayerInventory->SetVisibility(ESlateVisibility::Hidden);
}

void UMCInventoryVisualizerWidget::InitPlayerInventoryWidget(UMCPlayerInventory* InPlayerInventory)
{
	CurrentPlayerInventory = InPlayerInventory;
	InventoryGrid_Player->SpawnInventoryGrid(
		InPlayerInventory,
		this,
		InventorySlotWidgetClass,
		InventoryItemWidgetClass,
		{ 0, -1 }
	);
	InventoryGrid_Player->SetVisibility(ESlateVisibility::Visible);
	SetVisibility(DefaultSelfVisibility);
	SetFocus();
}
