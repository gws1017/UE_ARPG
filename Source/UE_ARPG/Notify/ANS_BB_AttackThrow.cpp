#include "Notify/ANS_BB_AttackThrow.h"
#include "Actor/Boss.h"
#include "Actor/Stone.h"
#include "Global.h"


FString UANS_BB_AttackThrow::GetNotifyName_Implementation() const
{
	return "AttackC";
}

void UANS_BB_AttackThrow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	ABoss* boss = Cast<ABoss>(MeshComp->GetOwner());
	CheckNull(boss);

	boss->SpawnStone();
}

void UANS_BB_AttackThrow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	ABoss* boss = Cast<ABoss>(MeshComp->GetOwner());
	CheckNull(boss);

	boss->AttackThrow();
}