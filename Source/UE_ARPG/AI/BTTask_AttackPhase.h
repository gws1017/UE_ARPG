#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackPhase.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API UBTTask_AttackPhase : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_AttackPhase();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
