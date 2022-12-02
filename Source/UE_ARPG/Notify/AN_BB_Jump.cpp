#include "Notify/AN_BB_Jump.h"
#include "Actor/Boss.h"
#include "Global.h"

FString UAN_BB_Jump::GetNotifyName_Implementation() const
{
	return "Jump";
}

void UAN_BB_Jump::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	ABoss* boss = Cast<ABoss>(MeshComp->GetOwner());
	CheckNull(boss);

	boss->AttackJump();
}
