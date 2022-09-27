#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimMontage.h"

#include "Actor/Weapon.h"
#include "Global.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	UHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	
	UHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Scene);
	UHelpers::CreateComponent<UBoxComponent>(this, &WeaponCollision, "ComatCollision", Scene);
	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::ComponentBeginOverlap);
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	WeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

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

void AWeapon::Begin_Attack()
{
}

void AWeapon::End_Attack()
{
	//Notify로 호출
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
	CheckTrue(bEquipping); //장착/탈착 중에는 공격할 수 없다
	CheckTrue(bAttacking);
	
	bAttacking = true;

	OwnerCharacter->PlayAnimMontage(AttackMontage);


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


