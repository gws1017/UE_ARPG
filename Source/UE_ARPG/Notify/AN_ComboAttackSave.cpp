#include "Notify/AN_ComboAttackSave.h"
#include "Actor/CPlayer.h"
#include "Global.h"

FString UAN_ComboAttackSave::GetNotifyName_Implementation() const
{
	return "ComboAttackSave";
}

void UAN_ComboAttackSave::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->ComboAttackSave();
}
