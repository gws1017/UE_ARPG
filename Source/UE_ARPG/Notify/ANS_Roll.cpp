#include "Notify/ANS_Roll.h"

#include "Actor/CPlayer.h"
#include "Global.h"

FString UANS_Roll::GetNotifyName_Implementation() const
{
	return "Roll";
}

void UANS_Roll::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);
	player->SetBlending(true);
	player->SetMovementState(EMovementState::EMS_Roll);
}

void UANS_Roll::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->SetMovementState(EMovementState::EMS_Normal);

	player->SetBlending(false);

}
