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

private:
	UPROPERTY()
		class ACPlayer* PlayerInstance;

public:

	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetPlayerExp() { return PlayerInstance->GetExp(); }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetPlayerLevelUpExp() { return PlayerInstance->GetLevelUpExp(); }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetPlayerLevel() const { return PlayerInstance->GetPlayerLevel(); }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetVigor() const { return PlayerInstance->GetVigor(); }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetEnduarance() const { return PlayerInstance->GetEnduarance(); }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetStrength() const { return PlayerInstance->GetStrength(); }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetMaxHP() const { return PlayerInstance->GetMaxHP(); }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetMaxStamina() const { return PlayerInstance->GetMaxStamina(); }
};
