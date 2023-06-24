#include "AI/BTTask_AttackPhase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Global.h"
#include "Actor/Boss.h"

UBTTask_AttackPhase::UBTTask_AttackPhase()
{
	NodeName = TEXT("AttackPhase");

}

EBTNodeResult::Type UBTTask_AttackPhase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ABoss* ControlledPawn = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (NULL == ControlledPawn) return EBTNodeResult::Failed;

	ControlledPawn->Attack();

	//공격에 성공하면 다음 공격패턴을 결정한다.
	ControlledPawn->SelectAttack();

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}