#include "Actor/LongSword.h"
#include "Actor/Weapon.h"
#include "Global.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"


ALongSword::ALongSword()
{

	USkeletalMesh* mesh;
	UHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Weapon/Mesh/SK_Sword.SK_Sword'");
	Mesh->SetSkeletalMesh(mesh);

	WeaponCollision->SetRelativeLocation(FVector(0, 0, -65));
	WeaponCollision->SetRelativeScale3D(FVector(0.1, 0.03, 1.7));
	
	UHelpers::GetAsset<UAnimMontage>(&DrawMontage, "AnimMontage'/Game/Character/Montage/Sword_Draw_Montage.Sword_Draw_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&SheathMontage, "AnimMontage'/Game/Character/Montage/Sword_Sheath_Montage.Sword_Sheath_Montage'");
	
	Damage = 5;
}
