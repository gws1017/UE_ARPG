#include "Notify/AN_HitEnd.h"

#include "Actor/CPlayer.h"
#include "Global.h"
FString UAN_HitEnd::GetNotifyName_Implementation() const
{
	return "Hit End";
}
void UAN_HitEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	ACPlayer* owner = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(owner);

	owner->HitEnd();
}