// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckStatus.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API UBTS_CheckStatus : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTS_CheckStatus();

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
