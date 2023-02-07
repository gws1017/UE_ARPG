#include "Actor/CAnimInstance.h"
#include "Actor/Weapon.h"
#include "Actor/Enemy.h"
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
	if (OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed <= 0.f) Speed = 0.f;
	Direction = UKismetAnimationLibrary::CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());

	//ĳ���� �������
	IICharacter* owner = Cast<IICharacter>(OwnerCharacter);
	if (!!owner)
	{
		AWeapon* weapon = owner->GetWeapon();
		if(!!weapon)
			bEquipped = weapon->GetEquipped();
	}

	//�� ��ü���� �ִ� ��
	AEnemy* enemy = Cast<AEnemy>(OwnerCharacter);

	if (!!enemy)
	{
		bAlert = enemy->GetAlerted();

	}

}
