#include "Actor/EnemySkeletonMelee.h"
#include "Actor/Enemy.h"
#include "Actor/Weapon.h"
#include "Actor/CPlayer.h"
#include "Actor/SkeletonSword.h"
#include "Global.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemySkeletonMelee::AEnemySkeletonMelee()
{

	USkeletalMesh* mesh;
	UHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Enemy/SkeletonMelee/Mesh/Corpse_Basic.Corpse_Basic'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeScale3D(FVector(0.8, 0.8, 0.8));

	GetCapsuleComponent()->SetRelativeScale3D(FVector(0.8, 0.8, 0.8));
	GetCapsuleComponent()->SetRelativeLocation(FVector(0, 0, -10));

	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	TSubclassOf<UAnimInstance> anim;
	UHelpers::GetClass(&anim, "AnimBlueprint'/Game/Enemy/SkeletonMelee/Animation/ABP_SkeletonMelee.ABP_SkeletonMelee_C'");
	GetMesh()->SetAnimInstanceClass(anim);

	UHelpers::GetAsset<UAnimMontage>(&DeathMontage, "AnimMontage'/Game/Enemy/SkeletonMelee/Montage/SM_Death_Montage.SM_Death_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&HitMontage, "AnimMontage'/Game/Enemy/SkeletonMelee/Montage/SM_Hit_Montage.SM_Hit_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&AttackMontage, "AnimMontage'/Game/Enemy/SkeletonMelee/Montage/SM_Attack_Montage.SM_Attack_Montage'");

	MaxHP = 15;
	HP = MaxHP;
	
	AgroSphere->InitSphereRadius(300.f);
	CombatSphere->InitSphereRadius(75.f);
}

void AEnemySkeletonMelee::BeginPlay()
{
	Super::BeginPlay();
	Weapon = AWeapon::Spawn<ASkeletonSword>(GetWorld(), this);
	Weapon->GetWeaponCollision()->OnComponentBeginOverlap.AddDynamic(this, &AEnemySkeletonMelee::WeaponBeginOverlap);

}

void AEnemySkeletonMelee::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		ACPlayer* player = Cast<ACPlayer>(OtherActor);
		if (player)
		{
			SetAlerted(true);
			MoveToTarget(player);
		}
	}
}

void AEnemySkeletonMelee::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && Alive())
	{
		ACPlayer* player = Cast<ACPlayer>(OtherActor);
		if (player)
		{
			MoveToSpawnLocation();
		}
	}

}

void AEnemySkeletonMelee::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!!OtherActor && Alive())
	{
		ACPlayer* player = Cast<ACPlayer>(OtherActor);
		if (player)
		{
			CombatTarget = player;

			if (CombatTarget->Alive())
			{
				Attack();
			}
			else
			{
				CLog::Print("player dead");
				CombatTarget = nullptr;
				GetWorldTimerManager().ClearTimer(AttackTimer);
				return;
			}
			
		}
	}
}

void AEnemySkeletonMelee::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!!OtherActor && Alive())
	{
		ACPlayer* player = Cast<ACPlayer>(OtherActor);
		if (player)
		{
			CombatTarget = nullptr;

			if (player->Alive())
			{
				MoveToTarget(player);
				GetWorldTimerManager().ClearTimer(AttackTimer);

			}
		}
	}
}

void AEnemySkeletonMelee::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* player = Cast<ACPlayer>(OtherActor);
	if (!!player)
	{
		//피격 이펙트 및 사운드 추가부분
		//사운드는 무기에서 얻고 피격 이펙트는 맞는 대상에서 가져온다
		UGameplayStatics::ApplyDamage(OtherActor, Weapon->GetDamage(), GetController(), Weapon, TSubclassOf<UDamageType>());

	}
}

void AEnemySkeletonMelee::Attack()
{
	CheckNull(CombatTarget);
	CheckFalse(Alive());
	PlayAnimMontage(AttackMontage);
	SetAttackTimer();
}
