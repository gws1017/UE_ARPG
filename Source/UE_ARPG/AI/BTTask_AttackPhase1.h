// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTTask_AttackPhase.h"
#include "BTTask_AttackPhase1.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API UBTTask_AttackPhase1 : public UBTTask_AttackPhase
{
	GENERATED_BODY()
	
public:
	UBTTask_AttackPhase1();

protected:
	//virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

private:
	//bool IsAttacking = false;
};
