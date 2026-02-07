#include "TextureHelper/TextureRenderingUtils.h"
#include "Engine/GameViewportClient.h"
using namespace BlueprintTextureHelper;


BlueprintTextureHelper::FScreenShot::FScreenShot(int32 InWidth, int32 InHeight, FOnScreenShotCaptured InOnTestScreenshotCaptured,
										   int32 InImageQuality, bool bInShowUI, bool bAddFilenameSuffix)
	: ScaledWidth(InWidth), ScaledHeight(InHeight), ImageQuality(InImageQuality)
{
	if (!UGameViewportClient::OnScreenshotCaptured().IsBound())
	{
		ScreenShotDelegateHandle = UGameViewportClient::OnScreenshotCaptured().AddRaw(
			this, 
			&FScreenShot::OnScreenshotCapturedInternal
		);
		FScreenshotRequest::RequestScreenshot(TEXT(""), bInShowUI, bAddFilenameSuffix);
		OnTestScreenshotCaptured = InOnTestScreenshotCaptured;
	}
}

void BlueprintTextureHelper::FScreenShot::OnScreenshotCapturedInternal(int32 SrcWidth, int32 SrcHeight, const TArray<FColor>& OrigBitmap)
{
	OnTestScreenshotCaptured.ExecuteIfBound(OrigBitmap, SrcWidth, SrcHeight);
	UGameViewportClient::OnScreenshotCaptured().Remove(ScreenShotDelegateHandle);
	delete this;
}
