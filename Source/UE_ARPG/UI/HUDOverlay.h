// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDOverlay.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API UHUDOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;


	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* HPBarBox;
	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* StaminaBarBox;

	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* BossInfoBox;

public:

	 void HideBossHPBar();
	 void ShowBossHPBar();

};
