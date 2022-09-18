#include "Notify/CAnimNotifyState_Attack.h"
#include "Actor/Weapon.h"
#include "Actor/CPlayer.h"
#include "Global.h"

FString UCAnimNotifyState_Attack::GetNotifyName_Implementation() const
{
	return "Attack";
}

void UCAnimNotifyState_Attack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->GetWeapon()->Begin_Attack();
}


void UCAnimNotifyState_Attack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->GetWeapon()->End_Attack();
}