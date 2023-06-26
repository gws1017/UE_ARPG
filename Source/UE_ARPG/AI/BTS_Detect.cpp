#include "AI/BTS_Detect.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Global.h"
#include "Actor/Boss.h"

UBTS_Detect::UBTS_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTS_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ABoss* ControlledPawn = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	
	ESkillType AttackNumber = ControlledPawn->GetAttackNumber();
	float radius = 0.f;

	switch (AttackNumber)
	{
	case ESkillType::EST_DefaultAttack:
	case ESkillType::EST_DefaultAttack2:
	case ESkillType::EST_AreaAttack:
		radius = 170.f;
		break;
	case ESkillType::EST_ThrowAttack:
	case ESkillType::EST_DropAttack:
		radius = 1000.f;
		break;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsRanged", ControlledPawn->IsRanged(radius));

}
