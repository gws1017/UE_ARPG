#include "Notify/ANS_BB_DropDown.h"

#include "Actor/Boss.h"
#include "Global.h"

FString UANS_BB_DropDown::GetNotifyName_Implementation() const
{
	return "DropDown";
}

void UANS_BB_DropDown::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	ABoss* boss = Cast<ABoss>(MeshComp->GetOwner());
	CheckNull(boss);

	boss->AttackDropDownBegin();

}

void UANS_BB_DropDown::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	ABoss* boss = Cast<ABoss>(MeshComp->GetOwner());
	CheckNull(boss);
	boss->AttackDropDownEnd();

}
