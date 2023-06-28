// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CharacterAbilityTables.generated.h"

USTRUCT(BlueprintType)
struct FCharacterAbilityData : public FTableRowBase
{
	GENERATED_BODY()
public:

	FCharacterAbilityData() : TotalHP(0.f), HPIncrease(0.f), 
		TotalStamina(0.f), StaIncrease(0.f),
		TotalDmgIncrease(0.f), DmgIncrease(0.f),
		LevelUpExp(0) {}

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float TotalHP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float HPIncrease;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float TotalStamina;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float StaIncrease;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float TotalDmgIncrease;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float DmgIncrease;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 LevelUpExp;

};

UCLASS()
class UE_ARPG_API ACharacterAbilityTables : public AActor
{
	GENERATED_BODY()

};
