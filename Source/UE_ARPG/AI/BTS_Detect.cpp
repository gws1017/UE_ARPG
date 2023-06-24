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
	float radius;
	//1.보스의 공격 종류를 데이터 테이블로만들자 -> 가독성도 늘것
	// -> 데미지 데이터 보스데이터를 따로만들어야함, 특정 몬스터를 참조했을때 
	// 얘가누군지 얘기술은 뭐가있는지 알아야하기때문 -> 여기서부턴 거의 DB를 활용하는 수준
	// DB까지 공부해서 작업을 처리하기엔 너무 과하고 해야할것이 너무많다
	//2. enum class로 코드 가독성을늘리자 -> 간단함
	//모든 공격을 범위 공격 판정을 하고있음 -> 사실 그게 의도한것임 ㅋㅋ
	//그럼 begin overlap은 안쓰나?
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
