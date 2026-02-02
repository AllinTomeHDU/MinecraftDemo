// Developer: Jichao Luo


#include "MCHotbarCursorWidget.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/PanelWidget.h"


void UMCHotbarCursorWidget::AnimateToNewParent(UPanelWidget* NewParent)
{
	TargetParent = NewParent;

	BeginTransitioning();

	// if new parent != null
	// begin transitioning to hidden
	// set parent to new parent
	// begin transitioning to showing
	// goto first line
}

void UMCHotbarCursorWidget::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);

	if (Player.GetAnimation() == DisappearAnimation)
	{
		TargetParent->AddChild(this);
		TargetParent = nullptr;

		if (bIsFirstTransition)
		{
			SetVisibility(VisibilityOnFirstNewParent);
			bIsFirstTransition = false;
		}

		PlayAnimation(AppearAnimation);
	}
	else if (Player.GetAnimation() == AppearAnimation)
	{
		bIsAnimating = false;
		if (TargetParent)
		{
			BeginTransitioning();
		}
	}
}

void UMCHotbarCursorWidget::BeginTransitioning()
{
	if (bIsAnimating) return;

	bIsAnimating = true;
	PlayAnimation(DisappearAnimation);
}
