// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StaminaBarUI.h"
#include "Actor/CPlayer.h"
#include "Global.h"

#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
void UStaminaBarUI::NativeOnInitialized()
{

}
void UStaminaBarUI::NativePreConstruct()
{
	StaminaBar->Percent = 1.0f;
	StaminaBar->FillColorAndOpacity = FLinearColor::Green;
	StaminaBar->PercentDelegate.BindUFunction(this, FName("GetStaminaPercent"));
}
void UStaminaBarUI::NativeDestruct()
{

}
float UStaminaBarUI::GetStaminaPercent()
{
	if (!OwnerCharacter)
		OwnerCharacter = Cast<ACPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!!OwnerCharacter)
		return (OwnerCharacter->GetStamina() / OwnerCharacter->GetMaxStamina());

	CLog::Log("StaminaPercent Error");
	return 0.0f;
}
