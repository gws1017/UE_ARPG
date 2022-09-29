#include "Actor/Enemy.h"
#include "Actor/Weapon.h"
#include "Global.h"

#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AEnemy::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount <= 0.f)
		return DamageAmount;

	if (HP - DamageAmount <= 0.f) //ü���� 0�̵ɶ� ������ Die�Լ� ȣ��
	{
		HP = FMath::Clamp(HP - DamageAmount, 0.0f, MaxHP);
		Die();
	}
	else //�Ϲ����� ������ ���
	{
		HP = FMath::Clamp(HP - DamageAmount, 0.0f, MaxHP);
	}
	UE_LOG(LogTemp, Display, L"Enemy Current HP : %f", HP);
	return DamageAmount;
}

void AEnemy::Hit()
{
	PlayAnimMontage(HitMontage);
}

void AEnemy::Die()
{
	CLog::Print("Enemy Die");
	PlayAnimMontage(DeathMontage);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(!!Weapon)
		Weapon->DeactivateCollision();
}

void AEnemy::Disappear()
{
	//������鼭 �ؾ��� �͵� �ۼ�
	Weapon->Destroy();
	Destroy();
}

void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay);
}

