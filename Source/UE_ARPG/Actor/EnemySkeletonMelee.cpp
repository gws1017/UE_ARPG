#include "Actor/EnemySkeletonMelee.h"
#include "Actor/Enemy.h"
#include "Actor/Weapon.h"
#include "Actor/SkeletonSword.h"
#include "Global.h"

#include "Components/CapsuleComponent.h"

AEnemySkeletonMelee::AEnemySkeletonMelee()
{
	USkeletalMesh* mesh;
	UHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Enemy/SkeletonMelee/Mesh/Corpse_Basic.Corpse_Basic'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeScale3D(FVector(0.8, 0.8, 0.8));

	GetCapsuleComponent()->SetRelativeScale3D(FVector(0.8, 0.8, 0.8));
	GetCapsuleComponent()->SetRelativeLocation(FVector(0, 0, -10));

	TSubclassOf<UAnimInstance> anim;
	UHelpers::GetClass(&anim, "AnimBlueprint'/Game/Enemy/SkeletonMelee/Animation/ABP_SkeletonMelee.ABP_SkeletonMelee_C'");
	GetMesh()->SetAnimInstanceClass(anim);

	UHelpers::GetAsset<UAnimMontage>(&DeathMontage, "AnimMontage'/Game/Enemy/SkeletonMelee/Montage/SM_Death_Montage.SM_Death_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&HitMontage, "AnimMontage'/Game/Enemy/SkeletonMelee/Montage/SM_Hit_Montage.SM_Hit_Montage'");
	
	HP = 15;
	MaxHP = 15;
}

void AEnemySkeletonMelee::BeginPlay()
{
	Super::BeginPlay();

	Weapon = AWeapon::Spawn<ASkeletonSword>(GetWorld(), this);
}
