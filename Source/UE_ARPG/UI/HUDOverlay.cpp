#include "UI/HUDOverlay.h"
#include "Global.h"

#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"

void UHUDOverlay::NativePreConstruct()
{
	Super::NativePreConstruct();
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

