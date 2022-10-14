#include "Notify/ANS_BossBear_RCollision.h"
#include "Actor/Boss.h"
#include "Global.h"


FString UANS_BossBear_RCollision::GetNotifyName_Implementation() const
{
	return "RCollsion";
}

void UANS_BossBear_RCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	ABoss* boss = Cast<ABoss>(MeshComp->GetOwner());
	CheckNull(boss);

	boss->Begin_Collision("RWeaponCollision");
}

void UANS_BossBear_RCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	ABoss* boss = Cast<ABoss>(MeshComp->GetOwner());
	CheckNull(boss);

	boss->End_Collision("RWeaponCollision");
}
