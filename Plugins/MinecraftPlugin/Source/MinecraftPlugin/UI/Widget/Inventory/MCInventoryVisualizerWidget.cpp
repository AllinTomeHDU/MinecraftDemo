// Developer: Jichao Luo


#include "MCInventoryVisualizerWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCInventorySlotWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCInventoryItemWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCInventoryGridWidget.h"
#include "MinecraftPlugin/Inventory/MCInventoryInterface.h"
#include "MinecraftPlugin/Inventory/MCPlayerInventory.h"
#include "MinecraftPlugin/Game/MCVoxelGameState.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "Blueprint/WidgetLayoutLibrary.h"


void UMCInventoryVisualizerWidget::TogglePlayerInventory(UMCPlayerInventory* InPlayerInventory, bool& OutIsMenuDisplayed)
{
	switch (VisualiserState)
	{
		case EMCInventoryVisualiserState::Hidden:
		{
			InitPlayerInventoryWidget(InPlayerInventory);
			OutIsMenuDisplayed = true;
			VisualiserState = EMCInventoryVisualiserState::ShowingPlayer;
			break;
		}
		case EMCInventoryVisualiserState::ShowingPlayer:
		{
			HidePlayerInventory();
			ChangePlayerInputsToWorld();
			OutIsMenuDisplayed = false;
			VisualiserState = EMCInventoryVisualiserState::Hidden;
			break;
		}
		case EMCInventoryVisualiserState::ShowingBoth:
		{
			HideSecondaryInventory();
			OutIsMenuDisplayed = InventoryGrid_Player->GetVisibility() == ESlateVisibility::Visible;
			VisualiserState = OutIsMenuDisplayed ? EMCInventoryVisualiserState::ShowingPlayer
												 : EMCInventoryVisualiserState::Hidden;
		}
	}
}

void UMCInventoryVisualizerWidget::ToggleBothInventories(UMCPlayerInventory* InPlayerInventory, TScriptInterface<IMCInventoryInterface> InOtherInventory, bool& OutIsMenuDisplayed)
{
	switch (VisualiserState)
	{
		case EMCInventoryVisualiserState::Hidden:
		{
			InitPlayerInventoryWidget(InPlayerInventory);
			InitOtherInventoryWidget(InOtherInventory);
			OutIsMenuDisplayed = true;
			VisualiserState = EMCInventoryVisualiserState::ShowingBoth;
			break;
		}
		case EMCInventoryVisualiserState::ShowingPlayer:
		{
			InitOtherInventoryWidget(InOtherInventory);
			OutIsMenuDisplayed = true;
			VisualiserState = EMCInventoryVisualiserState::ShowingBoth;
			break;
		}
		case EMCInventoryVisualiserState::ShowingBoth:
		{
			HidePlayerInventory();
			HideSecondaryInventory();
			OutIsMenuDisplayed = false;
			VisualiserState = EMCInventoryVisualiserState::Hidden;
		}
	}
}

void UMCInventoryVisualizerWidget::OnSlotButtonClicked(UMCInventorySlotWidget* InSlotWidget)
{
	if (!CurrentHeldItem && InSlotWidget->IsHoldingWidget())
	{
		// 从库中取出资产
		CurrentHeldItem = InSlotWidget->GetCurrentItemWidget();
		InSlotWidget->RemoveCurrentItemWidget();
		Overlay_HeldItem->AddChild(CurrentHeldItem);
		CurrentHeldItem->SetVisibility(ESlateVisibility::HitTestInvisible);
		Button_Close->SetIsEnabled(false);

		UMCItem* RemovedItem;
		InSlotWidget->GetAssociatedInventory()->RemoveFrom(
			InSlotWidget->GetRepresentedInventoryCoord(), 
			RemovedItem
		);
	}
	else if (CurrentHeldItem && !InSlotWidget->IsHoldingWidget())
	{
		// 将资产存入库中
		InSlotWidget->GetAssociatedInventory()->InsertInto(
			InSlotWidget->GetRepresentedInventoryCoord(),
			CurrentHeldItem->GetRepresentedItem()
		);
		CurrentHeldItem->RemoveFromParent();
		CurrentHeldItem->SetRenderTranslation(FVector2D::ZeroVector);
		CurrentHeldItem->SetVisibility(ESlateVisibility::Visible);
		InSlotWidget->AddItemWidget(CurrentHeldItem);
		CurrentHeldItem = nullptr;

		Button_Close->SetIsEnabled(true);
	}
}

void UMCInventoryVisualizerWidget::OnCloseButtonClicked()
{
	if (IsValid(CurrentHeldItem)) return;

	Hide();
}

void UMCInventoryVisualizerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Cast<AMCVoxelGameState>(GetWorld()->GetGameState())->SetInventoryVisualizer(this);

	SetVisibility(ESlateVisibility::Hidden);
	Overlay_OtherInventory->SetVisibility(DefaultSelfInvisibility);
	Overlay_PlayerInventory->SetVisibility(DefaultSelfInvisibility);

	Button_Close->OnClicked.AddDynamic(this, &ThisClass::OnCloseButtonClicked);
}

void UMCInventoryVisualizerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	TrackCurrentlyHeldItem();
}

FReply UMCInventoryVisualizerWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	/*
	* 当键盘事件设置为UIOnly时，输入事件需要在UI里监听
	*/
	if (VisualiserState == EMCInventoryVisualiserState::Hidden) return FReply::Unhandled();

	if (InKeyEvent.GetKey() == EKeys::E)
	{
		if (!GetWorld()->GetFirstPlayerController()) return FReply::Unhandled();
		if (CurrentHeldItem) return FReply::Unhandled();

		Hide();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UMCInventoryVisualizerWidget::Hide()
{
	switch (VisualiserState)
	{
	case EMCInventoryVisualiserState::Hidden:
		break;

	case EMCInventoryVisualiserState::ShowingPlayer:
		HidePlayerInventory();
		CurrentPlayerInventory = nullptr;
		ChangePlayerInputsToWorld();
		VisualiserState = EMCInventoryVisualiserState::Hidden;
		break;

	case EMCInventoryVisualiserState::ShowingBoth: // TODO: Identical to TogglePlayerInventory, ToggleBothInventories
		HidePlayerInventory();
		HideSecondaryInventory();
		CurrentPlayerInventory = nullptr;
		CurrentOtherInventory = nullptr;
		ChangePlayerInputsToWorld();
		VisualiserState = EMCInventoryVisualiserState::Hidden;
	}
}

void UMCInventoryVisualizerWidget::HideSecondaryInventory()
{
	InventoryGrid_Other->GetWrapBox_InventorySlot()->ClearChildren();
	Overlay_OtherInventory->SetVisibility(ESlateVisibility::Hidden);
	CurrentOtherInventory = nullptr;
}

void UMCInventoryVisualizerWidget::HidePlayerInventory()
{
	InventoryGrid_Player->GetWrapBox_InventorySlot()->ClearChildren();
	Overlay_PlayerInventory->SetVisibility(ESlateVisibility::Hidden);
	CurrentPlayerInventory = nullptr;
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
	Overlay_PlayerInventory->SetVisibility(DefaultSelfVisibility);
	SetVisibility(DefaultSelfVisibility);
	SetFocus();
}

void UMCInventoryVisualizerWidget::InitOtherInventoryWidget(TScriptInterface<IMCInventoryInterface> InOtherInventory)
{
	CurrentOtherInventory = InOtherInventory;
	InventoryGrid_Other->SpawnInventoryGrid(
		InOtherInventory,
		this,
		InventorySlotWidgetClass,
		InventoryItemWidgetClass
	);
	InventoryGrid_Other->SetVisibility(ESlateVisibility::Visible);
	Overlay_OtherInventory->SetVisibility(DefaultSelfVisibility);
	SetVisibility(DefaultSelfVisibility);
}

void UMCInventoryVisualizerWidget::TrackCurrentlyHeldItem()
{
	if (IsValid(CurrentHeldItem))
	{
		if (auto PC = GetWorld()->GetFirstPlayerController())
		{
			float DPI_X, DPI_Y;
			if (UWidgetLayoutLibrary::GetMousePositionScaledByDPI(PC, DPI_X, DPI_Y))
			{
				FVector2D ItemSize = CurrentHeldItem->GetDesiredSize();
				CurrentHeldItem->SetRenderTranslation(FVector2D(DPI_X, DPI_Y) - ItemSize * 0.5f);
			}
		}
	}
}

void UMCInventoryVisualizerWidget::ChangePlayerInputsToWorld()
{
	if (auto PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}
}
