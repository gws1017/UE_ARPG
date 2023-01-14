#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API AEnemyController : public AAIController
{
	GENERATED_BODY()
protected:

	UPROPERTY()
		class UBehaviorTree* BehaviorTree;
	UPROPERTY()
		class UBlackboardData* BlackboardData;

protected:

	bool bUseBehaviorTree = false;
public:

	AEnemyController();

public:

	UFUNCTION(BlueprintGetter)
		bool IsUseBehaviorTree() { return bUseBehaviorTree; }
	UFUNCTION(BlueprintCallable)
		void UseBehaviorTree() { bUseBehaviorTree = true; }


	
};
