#include "Actor/Weapon.h"
#include "Actor/Enemy.h"
#include "Actor/CPlayer.h"
#include "Interface/ICharacter.h"
#include "Global.h"

#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimMontage.h"



AWeapon::AWeapon()
	:Damage(5),StaminaCost(10)
{
	PrimaryActorTick.bCanEverTick = true;

	UHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	
	UHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Scene);
	UHelpers::CreateComponent<UBoxComponent>(this, &WeaponCollision, "ComatCollision", Scene);
	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	WeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	//�̰� �����༭ �ڲ�������
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	//�� ���⸦ ���� ĳ������ ��Ʈ�ѷ��� �����
	SetInstigator(OwnerCharacter->GetController());

	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), SheathSocket);
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Begin_Equip()
{
	bEquipped = true;
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), WeaponSocket);

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

void AWeapon::Begin_Collision()
{
	ActivateCollision();
}

void AWeapon::End_Collision()
{
	DeactivateCollision();
}

void AWeapon::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!!OtherActor)
	{
		IICharacter* other = Cast<IICharacter>(OtherActor);
		
		CheckNull(other);
		//�ǰ� ����Ʈ �� ���� �߰��κ�
		//����� ���⿡�� ��� �ǰ� ����Ʈ�� �´� ��󿡼� �����´�
		other->Hit();
		UGameplayStatics::ApplyDamage(OtherActor, Damage, WeaponInstigator, this, DamageTypeClass);

	}

}

void AWeapon::Equip()
{
	CheckTrue(bEquipped);//�̹� �������̸� �Լ� ����
	CheckTrue(bEquipping);//���� ���������̿��� ����

	bEquipping = true;

	OwnerCharacter->PlayAnimMontage(DrawMontage);

	SetInstigator(OwnerCharacter->GetController()); //���� ���� �� ��Ʋ�ѷ� ���� ���
	
}

void AWeapon::UnEquip()
{
	CheckFalse(bEquipped);
	CheckTrue(bEquipping);//���� �ִ����̿��� ����

	bEquipping = true;

	OwnerCharacter->PlayAnimMontage(SheathMontage);

}

void AWeapon::ActivateCollision()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::DeactivateCollision()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
