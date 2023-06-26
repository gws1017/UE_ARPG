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

	UPROPERTY(VisibleDefaultsOnly, Category = "Controlled Pawn")
		class ACPlayer* PlayerInstance;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
		class UPauseMenuUI* PauseMenuUI;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		class TSubclassOf<UPauseMenuUI> PauseMenuUIClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
		class ULevelUpUI* LevelUpUI;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		class TSubclassOf<ULevelUpUI> LevelUpUIClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
		class URestartMenuUI* RestartMenuUI;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		class TSubclassOf<URestartMenuUI> RestartMenuUIClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
		class UHUDOverlay* PlayerHUDOverlay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		class TSubclassOf<UHUDOverlay> HUDOverlayClass;

	bool bVisiblePauseMenu = false;
	bool bVisibleLevelUpUI = false;
	
	bool bReadyLevelUpUI = false;

	bool bGameInputMode = true;

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void TogglePauseMenu();
	UFUNCTION(BlueprintCallable)
		void ToggleLevelUpUI();

public:

	FORCEINLINE UHUDOverlay* GetHUD() { return PlayerHUDOverlay; }
	FORCEINLINE bool GetGameMode() { return bGameInputMode; }

	FORCEINLINE bool IsReadyLevelUpUI() { return bReadyLevelUpUI; }
	FORCEINLINE void SetReadylevelUpUI(bool val) {  bReadyLevelUpUI = val; }
	
	ACPlayer* GetPlayer();


private:

	UFUNCTION()
		void ShowGameUI(class UUserWidget* GameUI);
	UFUNCTION()
		void RemoveGameUI(class UUserWidget* GameUI);

	void SetGameOnlyMode();
	void SetUIOnlyMode();
	void SetGameAndUIMode();

public:

	UFUNCTION()
		void ShowRestartenu();
	UFUNCTION()
		void RemoveRestartMenu();

	UFUNCTION()
		void ShowPauseMenu();
	UFUNCTION()
		void RemovePauseMenu();
};
