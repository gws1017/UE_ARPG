#include "Actor/Boss.h"
#include "Global.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

ABoss::ABoss()
{
	USkeletalMesh* mesh;
	UHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Enemy/BossBear/Mesh/Boss_Bear.Boss_Bear'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -140));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	//GetMesh()->SetRelativeScale3D(FVector(0.8, 0.8, 0.8));

	GetCapsuleComponent()->SetCapsuleHalfHeight(150.f);
	GetCapsuleComponent()->SetCapsuleRadius(130.f);

	TSubclassOf<UAnimInstance> anim;
	UHelpers::GetClass(&anim, "AnimBlueprint'/Game/Enemy/BossBear/Animation/ABP_Bear.ABP_Bear_C'");
	GetMesh()->SetAnimInstanceClass(anim);

	//UHelpers::CreateComponent<USphereComponent>(this, &CollisionSphere,"WeaponCollision", GetCapsuleComponent());
	LWeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));
	LWeaponCollision->SetupAttachment(GetMesh(), FName("hand_lf"));
	LWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LWeaponCollision->SetRelativeLocation(FVector(8, 16, 1.5));
	LWeaponCollision->SetRelativeRotation(FRotator(-80, 0, -30));
	//¶÷´Ù?

	UHelpers::GetAsset<UAnimMontage>(&DeathMontage, "AnimMontage'/Game/Enemy/BossBear/Montage/Bear_Death_Montage.Bear_Death_Montage'");

	MaxHP = 1;// 50
	HP = MaxHP;

	AgroSphere->InitSphereRadius(300.f);
	CombatSphere->InitSphereRadius(170.f);
}

