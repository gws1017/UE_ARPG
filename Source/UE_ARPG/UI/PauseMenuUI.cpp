#include "UI/PauseMenuUI.h"
#include "Actor/CPlayerController.h"
#include "Actor/CPlayer.h"
#include "Global.h"

#include "Components/Button.h"



void UPauseMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuUI::OnClickResumeButton);
	QuitButton->OnClicked.AddDynamic(this, &UPauseMenuUI::OnClickQuitButton);

	Controller = Cast<ACPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerInstance = Controller->GetPawn<ACPlayer>();
}

void UPauseMenuUI::OnClickQuitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UPauseMenuUI::OnClickResumeButton()
{
	Controller->RemovePauseMenu();
}