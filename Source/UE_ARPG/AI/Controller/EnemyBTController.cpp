#include "AI/Controller/EnemyBTController.h"
#include "Global.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyBTController::AEnemyBTController()
{
	UHelpers::GetAsset(&BehaviorTree, "BehaviorTree'/Game/Enemy/BossBear/AI/BT_Boss.BT_Boss'");
	UHelpers::GetAsset(&BlackboardData, "BlackboardData'/Game/Enemy/BossBear/AI/BB_Boss.BB_Boss'");
	UseBehaviorTree();
}

void AEnemyBTController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CheckFalse(IsUseBehaviorTree());

	UBlackboardComponent* comp = Cast<UBlackboardComponent>(Blackboard);
	//UseBlackBoard는 TObjectPtr객체가 묵시적변환이 이루어지지 않는듯.
	if (UseBlackboard(BlackboardData, comp))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
