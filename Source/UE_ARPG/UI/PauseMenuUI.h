// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuUI.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API UPauseMenuUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* ResumeButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

public:
	UFUNCTION(BlueprintCallable)
		void OnClickQuitButton();
	UFUNCTION(BlueprintCallable)
		void OnClickResumeButton();

};
