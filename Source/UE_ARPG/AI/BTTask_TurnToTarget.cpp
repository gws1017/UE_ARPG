#include "AI/BTTask_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Actor/Enemy.h"
#include "Global.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = ("TurnToTarget");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AEnemy* ControlledPawn = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (NULL == ControlledPawn) return EBTNodeResult::Failed;

	CheckNullResult(ControlledPawn->GetTarget(),EBTNodeResult::Failed);

	FRotator rot = UKismetMathLibrary::FindLookAtRotation(ControlledPawn->GetActorLocation(),
		ControlledPawn->GetCombatTargetLocation());
	rot.Pitch = 0.f;
	ControlledPawn->SetActorRotation(rot);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}