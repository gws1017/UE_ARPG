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

	UPROPERTY(meta = (BindWidget))
		class UButton* ResumeButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

private:
	UPROPERTY()
	class ACPlayer* PlayerInstance;

	UPROPERTY()
		class ACPlayerController* Controller;

public:
	UFUNCTION(BlueprintCallable)
		void OnClickQuitButton();
	UFUNCTION(BlueprintCallable)
		void OnClickResumeButton();
	UFUNCTION(BlueprintCallable)
		FORCEINLINE int32 GetPlayerLevel() const { return PlayerInstance->GetPlayerLevel(); }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE int32 GetVigor() const { return PlayerInstance->GetVigor(); }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE int32 GetEnduarance() const { return PlayerInstance->GetEnduarance(); }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE int32 GetStrength() const { return PlayerInstance->GetStrength(); }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE int32 GetMaxHP() const { return PlayerInstance->GetMaxHP(); }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE int32 GetMaxStamina() const { return PlayerInstance->GetMaxStamina(); }
};
