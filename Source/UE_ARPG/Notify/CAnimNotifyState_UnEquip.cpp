#include "Notify/CAnimNotifyState_UnEquip.h"
#include "Actor/Weapon.h"
#include "Actor/CPlayer.h"
#include "Global.h"

FString UCAnimNotifyState_UnEquip::GetNotifyName_Implementation() const
{
	return "UnEquip";
}

void UCAnimNotifyState_UnEquip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->GetWeapon()->Begin_UnEquip();
}


void UCAnimNotifyState_UnEquip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->GetWeapon()->End_UnEquip();
}