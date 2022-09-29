#include "Notify/CAnimNotifyState_Collision.h"

#include "Actor/Weapon.h"
#include "Interface/ICharacter.h"
#include "Global.h"

FString UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}
void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	IICharacter* owner = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	owner->GetWeapon()->Begin_Collision();
}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	IICharacter* owner = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	owner->GetWeapon()->End_Collision();

}
