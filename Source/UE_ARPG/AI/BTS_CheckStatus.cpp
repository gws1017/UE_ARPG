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
	//처음한번 공격을정한다 -> 초기값설정해놓으면되지
	//공격이 성공하면 다음공격은 뭘로할지정한다. ->이거다 정답이야!

}