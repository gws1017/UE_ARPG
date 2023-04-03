#include "UI/HUDOverlay.h"
#include "Global.h"

#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"

void UHUDOverlay::NativeOnInitialized()
{
	HideBossHPBar();
	
}
void UHUDOverlay::NativePreConstruct()
{

}

void UHUDOverlay::NativeDestruct()
{

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

