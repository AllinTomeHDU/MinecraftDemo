// Copyright: Jichao Luo


#include "Room/MultiplayerRoomController.h"
#include "Room/MultiplayerRoomSubsystem.h"
#include "Room/Interface/CharacterSelectInterface.h"
#include "BlueprintOnlineHelperLibrary.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AMultiplayerRoomController::AMultiplayerRoomController()
{
	bReplicates = true;
}

void AMultiplayerRoomController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerRoomController, SelectedCharacter);
}

void AMultiplayerRoomController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (auto EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComp->BindAction(IA_Click, ETriggerEvent::Started, this, &ThisClass::OnClickAction);
	}
}

void AMultiplayerRoomController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		if (auto RoomSubsystem = GetGameInstance()->GetSubsystem<UMultiplayerRoomSubsystem>())
		{
			RoomSubsystem->RequestCloseTransitionWidget();
		}

		checkf(SelectedCharacterAnimInstance, TEXT("SelectedCharacterAnimInstance Is NULL"));
		TArray<AActor*> Characters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), Characters);
		for (auto& Tmp : Characters)
		{
			auto Chr = Cast<ACharacter>(Tmp);
			if (auto Mesh = Chr->GetMesh())
			{
				Mesh->SetAnimInstanceClass(SelectedCharacterAnimInstance);
			}
		}
	}

	if (auto EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedSubsystem->AddMappingContext(IMC_Default, 0);
	}

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	bShowMouseCursor = true;
}

void AMultiplayerRoomController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (!GetWorld() || GetWorld()->IsInSeamlessTravel()) return;
	if (bPendingKillOnTravel) return;

	// 高亮显示光标下未被选择的角色
	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECC_GameTraceChannel2, false, HitResult))
	{
		LastCursorActor = ThisCursorActor;
		ThisCursorActor = HitResult.GetActor();
		if (LastCursorActor != ThisCursorActor)
		{
			if (IsValid(ThisCursorActor) && IsValid(ThisCursorActor->GetClass()) &&
				ThisCursorActor->GetClass()->ImplementsInterface(UCharacterSelectInterface::StaticClass()))
			{
				if (!ICharacterSelectInterface::Execute_IsSelected(ThisCursorActor))
				{
					ICharacterSelectInterface::Execute_SetHightlight(ThisCursorActor, true);
				}
			}
			if (IsValid(LastCursorActor) && IsValid(LastCursorActor->GetClass()) &&
				LastCursorActor->GetClass()->ImplementsInterface(UCharacterSelectInterface::StaticClass()))
			{
				ICharacterSelectInterface::Execute_SetHightlight(LastCursorActor, false);
			}
		}
	}
	else
	{
		if (IsValid(LastCursorActor) && IsValid(LastCursorActor->GetClass()) &&
			LastCursorActor->GetClass()->ImplementsInterface(UCharacterSelectInterface::StaticClass()))
		{
			ICharacterSelectInterface::Execute_SetHightlight(LastCursorActor, false);
			LastCursorActor = nullptr;
			ThisCursorActor = nullptr;
		}
	}
}

void AMultiplayerRoomController::PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel)
{
	if (bIsSeamlessTravel)
	{
		bPendingKillOnTravel = true;
	}
	Super::PreClientTravel(PendingURL, TravelType, bIsSeamlessTravel);

	//if (auto RoomSubsystem = GetGameInstance()->GetSubsystem<UMultiplayerRoomSubsystem>())
	//{
	//	checkf(TravelTransitionWidgetClass, TEXT("TravelTransitionWidgetClass Is NULL"));
	//	RoomSubsystem->ShowTransitionWidget(TravelTransitionWidgetClass, 3.f, 10.f, true, 10);
	//}
}

void AMultiplayerRoomController::Logout()
{
	if (IsValid(SelectedCharacter))
	{
		Server_SelectCharacter(SelectedCharacter, false);
		Server_SetSelectedPlayer(SelectedCharacter, TEXT(""), TEXT(""));
	}
}

void AMultiplayerRoomController::OnClickAction()
{
	// 此处用自定义通道ECC_Character，避免修改ECC_Visibility通道对MotionMatching产生影响
	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECC_GameTraceChannel2, true, HitResult))
	{
		if (auto Chr = Cast<ACharacter>(HitResult.GetActor()))
		{
			if (Chr->GetClass()->ImplementsInterface(UCharacterSelectInterface::StaticClass()))
			{
				if (!ICharacterSelectInterface::Execute_IsSelected(Chr)) 
				{
					if (IsValid(SelectedCharacter))
					{
						Server_SelectCharacter(SelectedCharacter, false);
						Server_SetSelectedPlayer(SelectedCharacter, TEXT(""), TEXT(""));
					}
					FString PlayerAccount = UBlueprintOnlineHelperLibrary::GetOnlineUserID();
					FString PlayerName = UBlueprintOnlineHelperLibrary::GetOnlineUserName();
					Server_SelectCharacter(Chr, true);
					Server_SetSelectedPlayer(Chr, PlayerAccount, PlayerName);
					if (ThisCursorActor)
					{
						ICharacterSelectInterface::Execute_SetHightlight(ThisCursorActor, false);
					}

					if (auto GameSubsystem = GetGameInstance()->GetSubsystem<UMultiplayerRoomSubsystem>())
					{
						GameSubsystem->SetSavedCharacterClass(Chr->GetClass());
					}
				}
			}
		}
	}
}

void AMultiplayerRoomController::Server_SelectCharacter_Implementation(ACharacter* TargetCharacter, bool bIsSelected)
{
	if (TargetCharacter && TargetCharacter->GetClass()->ImplementsInterface(UCharacterSelectInterface::StaticClass()))
	{
		ICharacterSelectInterface::Execute_SetSelected(TargetCharacter, bIsSelected);
		if (bIsSelected)
		{
			SelectedCharacter = TargetCharacter;
		}
	}
}

void AMultiplayerRoomController::Server_SetSelectedPlayer_Implementation(ACharacter* TargetCharacter, const FString& PlayerAccout, const FString& PlayerName)
{
	if (TargetCharacter && TargetCharacter->GetClass()->ImplementsInterface(UCharacterSelectInterface::StaticClass()))
	{
		ICharacterSelectInterface::Execute_SetSelectedPlayer(TargetCharacter, PlayerAccout, PlayerName);
	}
}
