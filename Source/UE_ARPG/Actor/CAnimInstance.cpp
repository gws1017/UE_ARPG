#include "Actor/CAnimInstance.h"
#include "Actor/Weapon.h"
#include "Actor/Enemy.h"
#include "Interface/ICharacter.h"
#include "Global.h"

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

	Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());

	//ĳ���� �������
	IICharacter* owner = Cast<IICharacter>(OwnerCharacter);
	if (!!owner)
	{
		bEquipped = owner->GetWeapon()->GetEquipped();
	}

	//�� ��ü���� �ִ� ��
	AEnemy* enemy = Cast<AEnemy>(OwnerCharacter);

	if (!!enemy)
	{
		bAlert = enemy->GetAlerted();

	}

}
