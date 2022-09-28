#include "Notify/CAnimNotify_DeathEnd.h"
#include "Actor/Enemy.h"
#include "Global.h"

FString UCAnimNotify_DeathEnd::GetNotifyName_Implementation() const
{
	return "Death End";
}
void UCAnimNotify_DeathEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	AEnemy* enemy = Cast<AEnemy>(MeshComp->GetOwner());
	CheckNull(enemy);

	enemy->DeathEnd();
}
