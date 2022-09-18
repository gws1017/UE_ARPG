#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"

#include "Actor/Weapon.h"
#include "Global.h"

AWeapon* AWeapon::Spawn(UWorld* InWorld, ACharacter* InOwner)
{
	FActorSpawnParameters params;
	params.Owner = InOwner;

	return InWorld->SpawnActor<AWeapon>(params);
}

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	//우선 검으로 무기를 생성 
	// +이후 무기 추가할 경우 변경할 무기정보를 읽어와서 생성해야한다.
	UHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh");

	USkeletalMesh* mesh;
	UHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Weapon/Mesh/SK_Sword.SK_Sword'");
	Mesh->SetSkeletalMesh(mesh);

	UHelpers::GetAsset<UAnimMontage>(&DrawMontage, "AnimMontage'/Game/Character/Montage/Sword_Draw_Montage.Sword_Draw_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&SheathMontage, "AnimMontage'/Game/Character/Montage/Sword_Sheath_Montage.Sword_Sheath_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&AttackMontage, "AnimMontage'/Game/Character/Montage/Sword_Attack_Montage.Sword_Attack_Montage'");
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	//이거 안해줘서 자꾸터졌다
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), SheathSocket);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Begin_Equip()
{
	CLog::Print("bEquipped = true");
	bEquipped = true;
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), RHandSocket);
}

void AWeapon::End_Equip()
{
	bEquipping = false;
	CLog::Print("bEquipping false");

}

void AWeapon::Begin_UnEquip()
{
	bEquipped = false;
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), SheathSocket);

}

void AWeapon::End_UnEquip()
{
	bEquipping = false;
}

void AWeapon::Begin_Attack()
{
}

void AWeapon::End_Attack()
{
	//Notify로 호출
	bAttacking = false;
}

void AWeapon::Attack()
{
	CheckFalse(bEquipped);
	CheckTrue(bEquipping); //장착/탈착 중에는 공격할 수 없다
	CLog::Print("Attack Start : 90");
	CheckTrue(bAttacking);
	
	bAttacking = true;

	OwnerCharacter->PlayAnimMontage(AttackMontage);

	//충돌처리 추가할 것
}

void AWeapon::Equip()
{
	CheckTrue(bEquipped);//이미 착용중이면 함수 종료
	CheckTrue(bEquipping);//무기 꺼내는중이여도 종료

	bEquipping = true;

	OwnerCharacter->PlayAnimMontage(DrawMontage);
}

void AWeapon::UnEquip()
{
	CheckFalse(bEquipped);
	CheckTrue(bEquipping);//무기 넣는중이여도 종료

	bEquipping = true;

	OwnerCharacter->PlayAnimMontage(SheathMontage);

}


