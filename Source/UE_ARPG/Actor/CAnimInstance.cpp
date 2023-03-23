#include "Actor/CAnimInstance.h"
#include "Actor/Weapon.h"
#include "Actor/Enemy.h"
#include "Actor/CPlayer.h"
#include "Interface/ICharacter.h"
#include "Global.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

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
	Direction = UKismetAnimationLibrary::CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());
	
	//캐릭터 공통사항
	IICharacter* owner = Cast<IICharacter>(OwnerCharacter);
	if (!!owner)
	{
		AWeapon* weapon = owner->GetWeapon();
		if(!!weapon)
			bEquipped = weapon->GetEquipped();
	}

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (!!player)
	{
		bBlending = player->IsBlending();
	}


	//적 객체에만 있는 것
	AEnemy* enemy = Cast<AEnemy>(OwnerCharacter);

	if (!!enemy)
	{
		bAlert = enemy->GetAlerted();

	}

	
}
