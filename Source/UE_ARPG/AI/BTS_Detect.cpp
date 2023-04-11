#include "AI/BTS_Detect.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Global.h"
#include "Actor/Boss.h"
#include "Actor/CPlayer.h"

UBTS_Detect::UBTS_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTS_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ABoss* ControlledPawn = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (!!ControlledPawn)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject("TargetKey", ControlledPawn->GetTarget());
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject("TargetKey",NULL);
	}
	
	int32 AttackNumber = ControlledPawn->GetAttackNumber();
	float radius;
	switch (AttackNumber)
	{
	case 0:
	case 1:
	case 2:
		radius = 170.f;
		break;
	case 3:
	case 4:
		radius = 1000.f;
		break;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsRanged", ControlledPawn->IsRanged(radius));

}
