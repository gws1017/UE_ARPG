#include "UI/PauseMenuUI.h"
#include "Actor/CPlayerController.h"
#include "Global.h"

#include "Components/Button.h"



void UPauseMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuUI::OnClickResumeButton);
	QuitButton->OnClicked.AddDynamic(this, &UPauseMenuUI::OnClickQuitButton);
}

void UPauseMenuUI::OnClickQuitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UPauseMenuUI::OnClickResumeButton()
{
	ACPlayerController* Controller = Cast<ACPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	Controller->RemovePauseMenu();
}
