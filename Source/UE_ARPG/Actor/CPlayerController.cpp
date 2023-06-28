#include "Actor/CPlayerController.h"
#include "Actor/CPlayer.h"
#include "UI/PauseMenuUI.h"
#include "UI/HUDOverlay.h"
#include "UI/RestartMenuUI.h"
#include "UI/LevelUpUI.h"
#include "Global.h"

ACPlayerController::ACPlayerController()
{
}

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PauseMenuUI = Cast<UPauseMenuUI>(CreateWidget(GetWorld(), PauseMenuUIClass));
	RestartMenuUI = Cast<URestartMenuUI>(CreateWidget(GetWorld(), RestartMenuUIClass));
	PlayerHUDOverlay = Cast<UHUDOverlay>(CreateWidget(GetWorld(), HUDOverlayClass));
	LevelUpUI = Cast<ULevelUpUI>(CreateWidget(GetWorld(), LevelUpUIClass));
	PlayerInstance = Cast<ACPlayer>(GetPawn());

	
	auto InitializeUI = [](UUserWidget* GameUI, const ESlateVisibility& UIVisibility = ESlateVisibility::Visible) {
		if (GameUI) {
			GameUI->AddToViewport();
			GameUI->SetVisibility(UIVisibility);
		}
	};

	InitializeUI(PlayerHUDOverlay);
	InitializeUI(PauseMenuUI,ESlateVisibility::Hidden);
	InitializeUI(RestartMenuUI, ESlateVisibility::Hidden);

}

void ACPlayerController::TogglePauseMenu()
{
	if (bVisiblePauseMenu) 
		RemoveGameUI(PauseMenuUI);
	
	else
		ShowGameUI(PauseMenuUI);
	bVisiblePauseMenu = !bVisiblePauseMenu;
}

void ACPlayerController::ToggleLevelUpUI()
{
	if (bReadyInteraction) {
		if (bVisibleLevelUpUI)
			RemoveGameUI(LevelUpUI);
		else
		{
			ShowGameUI(LevelUpUI);
			GetPlayer()->SetHP(GetPlayer()->GetMaxHP());
			GetPlayer()->SetStartPoint(GetPlayer()->GetActorLocation());
			//몬스터 되살리는 코드여기에
		}
		bVisibleLevelUpUI = !bVisibleLevelUpUI;
	}
}


ACPlayer* ACPlayerController::GetPlayer()
{
	if (PlayerInstance)
		return PlayerInstance;
	else 
	{
		PlayerInstance = GetPawn<ACPlayer>();
		return PlayerInstance ? PlayerInstance : nullptr;
	}
}

void ACPlayerController::ShowGameUI(UUserWidget* GameUI)
{
	if (GameUI)
	{
		bShowMouseCursor = true;
		SetGameAndUIMode();
		if(!GameUI->IsInViewport())GameUI->AddToViewport();
		GameUI->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACPlayerController::RemoveGameUI(UUserWidget* GameUI)
{
	if (GameUI)
	{
		bShowMouseCursor = false;
		SetGameOnlyMode();
		if (GameUI->IsInViewport())GameUI->RemoveFromViewport();
	}
}

void ACPlayerController::SetGameOnlyMode()
{
	SetInputMode(FInputModeGameOnly{});
	bGameInputMode = true;
}

void ACPlayerController::SetUIOnlyMode()
{
	SetInputMode(FInputModeUIOnly{});
	bGameInputMode = false;
}

void ACPlayerController::SetGameAndUIMode()
{
	SetInputMode(FInputModeGameAndUI{});
	bGameInputMode = false;
}

void ACPlayerController::ShowRestartenu()
{
	ShowGameUI(RestartMenuUI);
}

void ACPlayerController::RemoveRestartMenu()
{
	RemoveGameUI(RestartMenuUI);
}

void ACPlayerController::ShowPauseMenu()
{
	ShowGameUI(PauseMenuUI);
}

void ACPlayerController::RemovePauseMenu()
{
	RemoveGameUI(PauseMenuUI); 
}
