#include "Notify/CAnimNotifyState_Attack.h"

#include "Actor/Weapon.h"
#include "Actor/Boss.h"
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

	AWeapon* weapon = owner->GetWeapon();

	if(!!weapon)
		weapon->Begin_Attack();
	else
	{
		ABoss* boss = Cast<ABoss>(owner);
		CheckNull(boss);

		boss->Begin_Attack();
	}
}


void UCAnimNotifyState_Attack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	IICharacter* owner = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	AWeapon* weapon = owner->GetWeapon();
	if (!!weapon)
		weapon->End_Attack();
	else 
	{
		ABoss* boss = Cast<ABoss>(owner);
		CheckNull(boss);

		boss->End_Attack();
	}

}
