#include "UI/RestartMenuUI.h"
#include "Actor/CPlayerController.h"
#include "Global.h"

#include "Components/Button.h"

void URestartMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	RestartButton->OnClicked.AddDynamic(this, &URestartMenuUI::OnClickRestartButton);
	QuitButton->OnClicked.AddDynamic(this, &URestartMenuUI::OnClickQuitButton);
}

void URestartMenuUI::OnClickQuitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void URestartMenuUI::OnClickRestartButton()
{
	ACPlayerController* Controller = Cast<ACPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	Controller->SetInputMode(FInputModeGameOnly{});
	
	
	FString CurrenLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrenLevelName));
}
