#include "Actor/CPlayerController.h"
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

	
	auto InitializeUI = [](UUserWidget* GameUI, const ESlateVisibility& UIVisibility = ESlateVisibility::Visible) {
		if (GameUI) {
			GameUI->AddToViewport();
			GameUI->SetVisibility(UIVisibility);
		}
	};

	InitializeUI(PlayerHUDOverlay);
	InitializeUI(PauseMenuUI,ESlateVisibility::Hidden);
	InitializeUI(RestartMenuUI, ESlateVisibility::Hidden);
	InitializeUI(LevelUpUI, ESlateVisibility::Hidden);

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
	if (bVisibleLevelUpUI)
		RemoveGameUI(LevelUpUI);
	else
		ShowGameUI(LevelUpUI);
	bVisibleLevelUpUI = !bVisibleLevelUpUI;

}

void ACPlayerController::ShowGameUI(UUserWidget* GameUI)
{
	if (GameUI)
	{
		bShowMouseCursor = true;
		SetGameAndUIMode();
		GameUI->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACPlayerController::RemoveGameUI(UUserWidget* GameUI)
{
	if (GameUI)
	{
		bShowMouseCursor = false;
		SetGameOnlyMode();
		GameUI->SetVisibility(ESlateVisibility::Hidden);
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
