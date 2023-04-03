#include "UI/HPBarUI.h"
#include "Actor/CPlayer.h"
#include "Actor/Enemy.h"
#include "Global.h"

#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "HPBarUI.h"

void UHPBarUI::NativeOnInitialized()
{
	HPBar->Percent = 1.0f;
	HPBar->FillColorAndOpacity = FLinearColor::Red;
}
void UHPBarUI::NativePreConstruct()
{
	if (!OwnerCharacter)
		OwnerCharacter = Cast<ACPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	//HPBar->PercentDelegate.BindUFunction(this, FName("GetHPPercent"));

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

	return 0.0f;
}

float UHPBarUI::GetBossHPPercent()
{
	CheckNullResult(OwnerCharacter,0.f);
	if (!Target)
		Target = OwnerCharacter->GetTarget();
	if (!!Target)
	{
		return (Target->GetHP() / Target->GetMaxHP());
	}
	

	return 0.0f;
}

