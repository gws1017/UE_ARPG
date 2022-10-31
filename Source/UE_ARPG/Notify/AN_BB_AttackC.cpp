#include "Notify/AN_BB_AttackC.h"
#include "Actor/Boss.h"
#include "Global.h"

FString UAN_BB_AttackC::GetNotifyName_Implementation() const
{
	return "AttackC";
}
void UAN_BB_AttackC::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	ABoss* boss = Cast<ABoss>(MeshComp->GetOwner());
	CheckNull(boss);

	boss->AttackC();

}
