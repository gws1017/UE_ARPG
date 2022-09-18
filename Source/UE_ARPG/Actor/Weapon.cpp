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

	//�켱 ������ ���⸦ ���� 
	// +���� ���� �߰��� ��� ������ ���������� �о�ͼ� �����ؾ��Ѵ�.
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

	//�̰� �����༭ �ڲ�������
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
	//Notify�� ȣ��
	bAttacking = false;
}

void AWeapon::Attack()
{
	CheckFalse(bEquipped);
	CheckTrue(bEquipping); //����/Ż�� �߿��� ������ �� ����
	CLog::Print("Attack Start : 90");
	CheckTrue(bAttacking);
	
	bAttacking = true;

	OwnerCharacter->PlayAnimMontage(AttackMontage);

	//�浹ó�� �߰��� ��
}

void AWeapon::Equip()
{
	CheckTrue(bEquipped);//�̹� �������̸� �Լ� ����
	CheckTrue(bEquipping);//���� ���������̿��� ����

	bEquipping = true;

	OwnerCharacter->PlayAnimMontage(DrawMontage);
}

void AWeapon::UnEquip()
{
	CheckFalse(bEquipped);
	CheckTrue(bEquipping);//���� �ִ����̿��� ����

	bEquipping = true;

	OwnerCharacter->PlayAnimMontage(SheathMontage);

}


