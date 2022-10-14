#include "Notify/ANS_BossBear_LCollision.h"
#include "Actor/Boss.h"
#include "Global.h"

FString UANS_BossBear_LCollision::GetNotifyName_Implementation() const
{
	return "LCollsion";
}

void UANS_BossBear_LCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	ABoss* boss = Cast<ABoss>(MeshComp->GetOwner());
	CheckNull(boss);

	boss->Begin_Collision("LWeaponCollision");
}

void UANS_BossBear_LCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	ABoss* boss = Cast<ABoss>(MeshComp->GetOwner());
	CheckNull(boss);

	boss->End_Collision("LWeaponCollision");
}

