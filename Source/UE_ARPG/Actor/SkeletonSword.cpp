#include "Actor/SkeletonSword.h"
#include "Global.h"

#include "Components/BoxComponent.h"


ASkeletonSword::ASkeletonSword() 
{
	USkeletalMesh* mesh;
	UHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Weapon/Mesh/Skeleton_Sword_Mesh.Skeleton_Sword_Mesh'");
	Mesh->SetSkeletalMesh(mesh);

	WeaponCollision->SetRelativeLocation(FVector(45, -4, 0));
	WeaponCollision->SetRelativeScale3D(FVector(1.1, 0.25, 0.05));

	SheathSocket = "SwordSocket";

	bEquipped = true;
	bEquipping = false;

	Damage = 3;
}
