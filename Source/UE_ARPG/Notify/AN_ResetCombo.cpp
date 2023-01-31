#include "Notify/AN_ResetCombo.h"
#include "Actor/CPlayer.h"
#include "Global.h"

FString UAN_ResetCombo::GetNotifyName_Implementaion() const
{
	return "ResetCombo";
}
void UAN_ResetCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->ResetCombo();
}
