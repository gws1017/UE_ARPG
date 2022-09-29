#include "Notify/CAnimNotifyState_UnEquip.h"
#include "Actor/Weapon.h"
#include "Interface/ICharacter.h"
#include "Global.h"

FString UCAnimNotifyState_UnEquip::GetNotifyName_Implementation() const
{
	return "UnEquip";
}

void UCAnimNotifyState_UnEquip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration,EventReference);
	CheckNull(MeshComp);

	IICharacter* owner = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	owner->GetWeapon()->Begin_UnEquip();
}


void UCAnimNotifyState_UnEquip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation,EventReference);
	CheckNull(MeshComp);

	IICharacter* owner = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	owner->GetWeapon()->End_UnEquip();
}