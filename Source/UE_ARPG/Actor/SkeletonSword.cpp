#include "Components/BoxComponent.h"

#include "Actor/SkeletonSword.h"
#include "Global.h"

ASkeletonSword::ASkeletonSword() 
{
	USkeletalMesh* mesh;
	UHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Weapon/Mesh/Skeleton_Sword_Mesh.Skeleton_Sword_Mesh'");
	Mesh->SetSkeletalMesh(mesh);

	//UHelpers::GetAsset<UAnimMontage>(&AttackMontage, "");

	WeaponCollision->SetRelativeLocation(FVector(45, -4, 0));
	WeaponCollision->SetRelativeScale3D(FVector(1.1, 0.25, 0.05));

	SheathSocket = "SwordSocket";

	Damage = 3;
}
