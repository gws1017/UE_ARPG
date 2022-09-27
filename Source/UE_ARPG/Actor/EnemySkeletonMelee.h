// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Enemy.h"
#include "EnemySkeletonMelee.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API AEnemySkeletonMelee : public AEnemy
{
	GENERATED_BODY()
	
public:
	AEnemySkeletonMelee();

protected:
	virtual void BeginPlay() override;
};
