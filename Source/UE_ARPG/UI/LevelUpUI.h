// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelUpUI.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API ULevelUpUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	class UBorder* VigBorder;
	UPROPERTY(meta = (BindWidget))
		class UBorder* EndureBorder;
	UPROPERTY(meta = (BindWidget))
		class UBorder* StrBorder;
private:
	UPROPERTY()
		class ACPlayer* PlayerInstance;

	UPROPERTY()
		class UMyGameInstance* GameInstance;

		int32 SelectUINumber;
		int32 TargetExp;

	UPROPERTY()
		TArray<UBorder*> AbilityArray;
	UPROPERTY()
		TArray<int32> LevelUpCount;

private:

	void SelectAbility();
	void UpdateTargetExp();
	UFUNCTION(BlueprintCallable)
		void OnUpKey();
	UFUNCTION(BlueprintCallable)
		void OnDownKey();
	UFUNCTION(BlueprintCallable)
		void OnLeftKey();
	UFUNCTION(BlueprintCallable)
		void OnRightKey();
public:

	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetPlayerExp() { return PlayerInstance->GetExp(); }
	UFUNCTION(BlueprintPure)
		int32 GetChangePlayerExp();
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetPlayerLevelUpExp() { return TargetExp; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetPlayerLevel() const { return PlayerInstance->GetPlayerLevel(); }
	UFUNCTION(BlueprintPure)
		int32 GetChangePlayerLevel();

	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetVigor() const { return PlayerInstance->GetVigor(); }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetChangeVigor() const { return PlayerInstance->GetVigor()+LevelUpCount[0]; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetEnduarance() const { return PlayerInstance->GetEnduarance(); }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetChangeEnduarance() const { return PlayerInstance->GetEnduarance() + LevelUpCount[1]; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetStrength() const { return PlayerInstance->GetStrength(); }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetChangeStrength() const { return PlayerInstance->GetStrength() + LevelUpCount[2]; }
	
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetMaxHP() const { return static_cast<int32>(PlayerInstance->GetMaxHP()); }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetMaxStamina() const { return static_cast<int32>(PlayerInstance->GetMaxStamina()); }
	UFUNCTION(BlueprintPure)
		int32 GetChangeMaxHP();
	UFUNCTION(BlueprintPure)
		int32 GetChangeMaxStamina();

	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetPlayerDamage() const { return static_cast<int32>(PlayerInstance->GetDamage()); }
	UFUNCTION(BlueprintPure)
		 int32 GetChangePlayerDamage();

};
