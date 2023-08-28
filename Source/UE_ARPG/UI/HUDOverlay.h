// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Actor/CPlayer.h"
#include "HUDOverlay.generated.h"


UCLASS()
class UE_ARPG_API UHUDOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* HPBarBox;
	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* StaminaBarBox;

	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* BossInfoBox;

private:
	UPROPERTY()
		class ACPlayer* PlayerInstance;
public:

	 void HideBossHPBar();
	 void ShowBossHPBar();

	 UFUNCTION(BlueprintPure)
		 FORCEINLINE int32 GetPlayerExp() { return PlayerInstance->GetExp(); }

};
