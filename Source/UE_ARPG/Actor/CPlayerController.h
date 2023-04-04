// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACPlayerController();

	UPROPERTY(BlueprintReadOnly, Category = "UI")
		class UPauseMenuUI* PauseMenuUI;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		class TSubclassOf<UPauseMenuUI> PauseMenuUIClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
		class URestartMenuUI* RestartMenuUI;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		class TSubclassOf<URestartMenuUI> RestartMenuUIClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
		class UHUDOverlay* PlayerHUDOverlay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		class TSubclassOf<UHUDOverlay> HUDOverlayClass;

	bool bVisiblePauseMenu = false;

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

	UFUNCTION()
		void ShowPauseMenu();
	UFUNCTION()
		void RemovePauseMenu();

	UFUNCTION()
		void ShowRestartenu();
	UFUNCTION()
		void RemoveRestartMenu();

	FORCEINLINE UHUDOverlay* GetHUD() { return PlayerHUDOverlay; }
};
