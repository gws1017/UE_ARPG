#include "AI/BTTask_AttackPhase1.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Global.h"
#include "Actor/Boss.h"

UBTTask_AttackPhase1::UBTTask_AttackPhase1()
{
	NodeName = TEXT("AttackPhase1");
}

