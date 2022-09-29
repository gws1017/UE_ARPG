#include "Notify/CAnimNotifyState_Attack.h"

#include "Actor/Weapon.h"
#include "Interface/ICharacter.h"
#include "Global.h"

FString UCAnimNotifyState_Attack::GetNotifyName_Implementation() const
{
	return "Attack";
}

void UCAnimNotifyState_Attack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	IICharacter* owner = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	owner->GetWeapon()->Begin_Attack();
}


void UCAnimNotifyState_Attack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	IICharacter* owner = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	owner->GetWeapon()->End_Attack();
}