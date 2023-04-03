#include "Actor/CPlayerController.h"
#include "UI/PauseMenuUI.h"
#include "UI/HUDOverlay.h"
#include "Global.h"

ACPlayerController::ACPlayerController()
{
	//UHelpers::GetClass(&PauseMenuUIClass, "WidgetBlueprint'/Game/Character/UI/BP_PauseMenuUI.BP_PauseMenuUI_C'");
	//UHelpers::GetClass(&HUDOverlayClass, "WidgetBlueprint'/Game/Character/UI/BP_HUDOverlay.BP_HUDOverlay_C'");
	

}

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PauseMenuUI = Cast<UPauseMenuUI>(CreateWidget(GetWorld(), PauseMenuUIClass));
	PlayerHUDOverlay = Cast<UHUDOverlay>(CreateWidget(GetWorld(), HUDOverlayClass));

	if (!!PlayerHUDOverlay) 
	{
		PlayerHUDOverlay->AddToViewport();
	}
	if (PauseMenuUI) 
	{
		PauseMenuUI->AddToViewport();
		PauseMenuUI->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ACPlayerController::TogglePauseMenu()
{
	if (bVisiblePauseMenu)
		RemovePauseMenu();
	else
		ShowPauseMenu(); 
}

void ACPlayerController::ShowPauseMenu()
{
	if (PauseMenuUI)
	{
		bVisiblePauseMenu = true;
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly{});
		PauseMenuUI->SetVisibility(ESlateVisibility::Visible);
	}
	
}

void ACPlayerController::RemovePauseMenu()
{
	if (PauseMenuUI)
	{
		bVisiblePauseMenu = false;
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly{});
		PauseMenuUI->SetVisibility(ESlateVisibility::Hidden);
	}

}
