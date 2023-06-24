#include "AI/BTS_CheckStatus.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Global.h"
#include "Actor/CPlayer.h"
#include "Actor/Boss.h"

UBTS_CheckStatus::UBTS_CheckStatus()
{
	NodeName = TEXT("CheckStatus");
	Interval = 1.0f;
}

void UBTS_CheckStatus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ABoss* ControlledPawn = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	
	CheckNull(ControlledPawn);

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("CurrentHP", ControlledPawn->GetCurrentHP());

	ControlledPawn->CalculateBossPhase();
	OwnerComp.GetBlackboardComponent()->SetValueAsInt("PhaseNumber", ControlledPawn->GetBossPhase());
	OwnerComp.GetBlackboardComponent()->SetValueAsObject("TargetKey", ControlledPawn->GetTarget());
	//ó���ѹ� ���������Ѵ� -> �ʱⰪ�����س��������
	//������ �����ϸ� ���������� �����������Ѵ�. ->�̰Ŵ� �����̾�!

}