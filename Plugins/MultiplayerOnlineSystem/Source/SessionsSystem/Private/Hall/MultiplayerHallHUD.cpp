// Copyright: Jichao Luo


#include "Hall/MultiplayerHallHUD.h"
#include "Core/MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

#ifdef CLIENT_SYSTEM
#include "ClientLocalPlayerSubsystem.h"
#include "ClientWidgetBase.h"
#endif


void AMultiplayerHallHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	if (auto PC = World->GetFirstPlayerController())
	{
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}

	if (auto SessionsSubsystem = GetGameInstance()->GetSubsystem<UMultiplayerSessionsSubsystem>())
	{
		if (SessionsSubsystem->GetGameEnterMode() == EGameEnterMode::Startup)
		{
		#ifdef CLIENT_SYSTEM
			auto LocalPlayer = World->GetFirstLocalPlayerFromController();
			if (!IsValid(LocalPlayer)) return;
			if (auto ClientSubsystem = LocalPlayer->GetSubsystem<UClientLocalPlayerSubsystem>())
			{
				if (ClientSubsystem->IsLoginComplete())
				{
					CreateHallWidget();
				}
				else
				{
					checkf(RegisterLoginWidgetClass, TEXT("RegisterLoginWidgetClass IS NULL"));
					RegisterLoginWidget = CreateWidget<UUserWidget>(GetWorld(), RegisterLoginWidgetClass);
					if (IsValid(RegisterLoginWidget))
					{
						RegisterLoginWidget->AddToViewport();
						if (auto ClientWidget = Cast<UClientWidgetBase>(RegisterLoginWidget))
						{
							ClientWidget->OnClientWidgetDestruct.AddUObject(this, &ThisClass::OnRegisterLoginWidgetDestruct);
						}
					}
				}
			}
		#else
			CreateMainPageWidget();
		#endif
		}
		else
		{
			CreateHallWidget();
		}
	}
}

#ifdef CLIENT_SYSTEM
void AMultiplayerHallHUD::OnRegisterLoginWidgetDestruct()
{
	CreateMainPageWidget();
}
#endif

void AMultiplayerHallHUD::CreateMainPageWidget()
{
	checkf(MainPageWidgetClass, TEXT("MainPageWidgetClass is nullptr"));
	MainPageWidget = CreateWidget<UUserWidget>(GetWorld(), MainPageWidgetClass);
	MainPageWidget->AddToViewport();
}

void AMultiplayerHallHUD::CreateHallWidget()
{
	checkf(HallWidgetClass, TEXT("HallWidgetClass is nullptr"));
	HallWidget = CreateWidget<UUserWidget>(GetWorld(), HallWidgetClass);
	HallWidget->AddToViewport();
}


