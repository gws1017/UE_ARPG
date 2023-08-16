#include "UI/TitleUI.h"
#include "Utilities/MySaveGame.h"
#include "Global.h"

#include "Components/Button.h"

void UTitleUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	if (!Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex)))
		ContinueBtn->SetIsEnabled(false);
}

void UTitleUI::OnClickQuitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UTitleUI::OnClickNewGameButton()
{
	UGameplayStatics::OpenLevel(this, "InGameLevel");
}

void UTitleUI::OnClickContinueButton()
{
	UGameplayStatics::OpenLevel(this, "InGameLevel");
}
