// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Controller/EnemyController.h"
#include "EnemyBTController.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API AEnemyBTController : public AEnemyController
{
	GENERATED_BODY()

public:

	AEnemyBTController();

public:

	virtual void OnPossess(APawn* InPawn) override;
};
