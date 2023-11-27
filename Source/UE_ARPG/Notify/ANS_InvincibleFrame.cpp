#include "Notify/ANS_InvincibleFrame.h"

#include "Actor/CPlayer.h"
#include "Global.h"

FString UANS_InvincibleFrame::GetNotifyName_Implementation() const
{
	return "InvincibleFrame";
}

void UANS_InvincibleFrame::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);
	CLog::Log("Invincible On");
	player->SetPlayerState(EPlayerStat::EPS_Invincible);
}

void UANS_InvincibleFrame::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);
	CLog::Log("Invincible Off");
	player->SetPlayerState(EPlayerStat::EPS_Normal);

}
