#include "Components/CapsuleComponent.h"

#include "Actor/EnemySkeletonMelee.h"
#include "Global.h"

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

	//GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
}
