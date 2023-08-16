#include "Actor/Controller/TitlePlayerController.h"
#include "UI/TitleUI.h"
#include "Global.h"

ATitlePlayerController::ATitlePlayerController()
{
}

void ATitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	TitleUI = Cast<UTitleUI>(CreateWidget(GetWorld(), TitleUIClass));

	TitleUI->AddToViewport();

	FInputModeUIOnly InputMode;

	SetInputMode(InputMode);
	bShowMouseCursor = true;
}
