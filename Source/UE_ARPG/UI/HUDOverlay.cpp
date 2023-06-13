#include "UI/HUDOverlay.h"
#include "Actor/CPlayer.h"
#include "Global.h"

#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"

void UHUDOverlay::NativePreConstruct()
{
	Super::NativePreConstruct();

	PlayerInstance = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	HideBossHPBar();
}

void UHUDOverlay::HideBossHPBar()
{
	for (auto widget : BossInfoBox->GetAllChildren())
	{
		widget->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UHUDOverlay::ShowBossHPBar()
{
	for (auto widget : BossInfoBox->GetAllChildren())
	{
		widget->SetVisibility(ESlateVisibility::Visible);
	}

}

