#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
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

	UHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");

	//�켱 ������ ���⸦ ���� 
	// +���� ���� �߰��� ��� ������ ���������� �о�ͼ� �����ؾ��Ѵ�.
	UHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Scene);

	USkeletalMesh* mesh;
	UHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Weapon/Mesh/SK_Sword.SK_Sword'");
	Mesh->SetSkeletalMesh(mesh);

	UHelpers::CreateComponent<UBoxComponent>(this, &WeaponCollision, "ComatCollision", Scene);
	WeaponCollision->SetRelativeLocation(FVector(0, 0, -70));
	WeaponCollision->SetRelativeScale3D(FVector(0.1,0.1,1.5));
	
	

	UHelpers::GetAsset<UAnimMontage>(&DrawMontage, "AnimMontage'/Game/Character/Montage/Sword_Draw_Montage.Sword_Draw_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&SheathMontage, "AnimMontage'/Game/Character/Montage/Sword_Sheath_Montage.Sword_Sheath_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&AttackMontage, "AnimMontage'/Game/Character/Montage/Sword_Attack_Montage.Sword_Attack_Montage'");
	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::ComponentBeginOverlap);
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	WeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

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
	bEquipped = true;
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), RHandSocket);
}

void AWeapon::End_Equip()
{
	bEquipping = false;

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

void AWeapon::Begin_Collision()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::End_Collision()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CLog::Print("Weapon BeginOverlap!");
}

void AWeapon::Attack()
{
	CheckFalse(bEquipped);
	CheckTrue(bEquipping); //����/Ż�� �߿��� ������ �� ����
	CheckTrue(bAttacking);
	
	bAttacking = true;

	OwnerCharacter->PlayAnimMontage(AttackMontage);


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


