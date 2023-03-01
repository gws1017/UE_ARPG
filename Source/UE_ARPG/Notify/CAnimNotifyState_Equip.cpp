#include "Notify/CAnimNotifyState_Equip.h"
#include "Actor/Weapon.h"
#include "Interface/ICharacter.h"
#include "Global.h"

FString UCAnimNotifyState_Equip::GetNotifyName_Implementation() const
{
	return "Equip";
}

void UCAnimNotifyState_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	IICharacter* player = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(player);
	CheckNull(player->GetWeapon());
	player->GetWeapon()->Begin_Equip();
}

void UCAnimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	IICharacter* player = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(player);
	CheckNull(player->GetWeapon());

	player->GetWeapon()->End_Equip();
}
