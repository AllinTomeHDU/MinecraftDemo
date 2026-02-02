// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "../MCUserWidgetBase.h"
#include "MCHotbarCursorWidget.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFTPLUGIN_API UMCHotbarCursorWidget : public UMCUserWidgetBase
{
	GENERATED_BODY()
	
public:
	void AnimateToNewParent(UPanelWidget* NewParent);
	
protected:
	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;

private:
	// 开始过渡动画
	void BeginTransitioning();

private:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* AppearAnimation;		// 淡入/显示光标

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* DisappearAnimation;	// 淡出/隐藏光标

	UPROPERTY(EditDefaultsOnly)
	ESlateVisibility VisibilityOnFirstNewParent = ESlateVisibility::SelfHitTestInvisible;

	UPROPERTY()
	UPanelWidget* TargetParent;

	bool bIsAnimating = false;
	bool bIsFirstTransition = true;
	
};
