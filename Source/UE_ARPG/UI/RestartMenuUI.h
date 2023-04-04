// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RestartMenuUI.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API URestartMenuUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeOnInitialized() override;


	UPROPERTY(meta = (BindWidget))
		class UButton* RestartButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

public:
	UFUNCTION(BlueprintCallable)
		void OnClickQuitButton();
	UFUNCTION(BlueprintCallable)
		void OnClickRestartButton();
};
