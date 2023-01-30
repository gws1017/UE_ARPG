// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPBarUI.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API UHPBarUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* HBox;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HPBar;


	UPROPERTY(VisibleAnywhere)
		class ACPlayer* OwnerCharacter;

public:

	UFUNCTION(BlueprintPure)
		float GetHPPercent();

};
