#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

#include "Actor/Weapon.h"
#include "Actor/LongSword.h"
#include "Global.h"

ALongSword::ALongSword()
{

	USkeletalMesh* mesh;
	UHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Weapon/Mesh/SK_Sword.SK_Sword'");
	Mesh->SetSkeletalMesh(mesh);

	WeaponCollision->SetRelativeLocation(FVector(0, 0, -70));
	WeaponCollision->SetRelativeScale3D(FVector(0.1, 0.1, 1.5));
	
	UHelpers::GetAsset<UAnimMontage>(&DrawMontage, "AnimMontage'/Game/Character/Montage/Sword_Draw_Montage.Sword_Draw_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&SheathMontage, "AnimMontage'/Game/Character/Montage/Sword_Sheath_Montage.Sword_Sheath_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&AttackMontage, "AnimMontage'/Game/Character/Montage/Sword_Attack_Montage.Sword_Attack_Montage'");
}
