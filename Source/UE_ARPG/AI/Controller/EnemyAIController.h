// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyAIController();
public:

	bool IsUseBehaviorTree() { return bUseBehaviorTree; }
	void UseBehaviorTree() { bUseBehaviorTree = true; }
protected:

	bool bUseBehaviorTree = false;
protected:

	UPROPERTY()
		class UBehaviorTree* BehaviorTree;
	UPROPERTY()
		class UBlackboardData* BlackboardData;
	
};
