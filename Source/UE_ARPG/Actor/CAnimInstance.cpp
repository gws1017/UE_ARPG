#include "GameFramework/Character.h"

#include "Actor/CPlayer.h"
#include "Actor/Weapon.h"
#include "Actor/CAnimInstance.h"
#include "Global.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());

}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();

	bEquipped = Cast<ACPlayer>(OwnerCharacter)->GetWeapon()->GetEquipped();

}
