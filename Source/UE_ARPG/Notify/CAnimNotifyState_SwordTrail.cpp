#include "Notify/CAnimNotifyState_SwordTrail.h"
#include "Actor/CPlayer.h"
#include "Particles/ParticleSystem.h"
#include "Global.h"

UCAnimNotifyState_SwordTrail::UCAnimNotifyState_SwordTrail(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	UParticleSystem* particle;
	UHelpers::GetAsset<UParticleSystem>(&particle, "ParticleSystem'/Game/Character/Effects/P_MeleeTrails_Regular.P_MeleeTrails_Regular'");

	PSTemplate = particle;

	FirstSocketName = "Sword_Trail01";
	SecondSocketName = "Sword_Trail02";
}

FString UCAnimNotifyState_SwordTrail::GetNotifyName_Implementation() const
{
	return "SwordTrail";
}
