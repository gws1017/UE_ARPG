#include "UI/HPBarUI.h"
#include "Actor/CPlayer.h"
#include "Global.h"

#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "HPBarUI.h"

void UHPBarUI::NativeOnInitialized()
{
}
void UHPBarUI::NativePreConstruct()
{
	HPBar->Percent = 1.0f;
	HPBar->FillColorAndOpacity = FLinearColor::Red;
	HPBar->PercentDelegate.BindUFunction(this, FName("GetHPPercent"));

}

void UHPBarUI::NativeDestruct()
{

}


float UHPBarUI::GetHPPercent()
{
	if (!OwnerCharacter)
		OwnerCharacter = Cast<ACPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(!!OwnerCharacter)
		return (OwnerCharacter->GetHP() / OwnerCharacter->GetMaxHP());

	CLog::Log("StaminaPercent Error");
	return 0.0f;
}

