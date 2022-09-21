#include "Notify/CAnimNotifyState_Equip.h"
#include "Actor/Weapon.h"
#include "Actor/CPlayer.h"
#include "Global.h"

FString UCAnimNotifyState_Equip::GetNotifyName_Implementation() const
{
	return "Equip";
}

void UCAnimNotifyState_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->GetWeapon()->Begin_Equip();
}

void UCAnimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->GetWeapon()->End_Equip();
}
