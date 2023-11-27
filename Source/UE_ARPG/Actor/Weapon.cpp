#include "Actor/Weapon.h"
#include "Actor/Enemy.h"
#include "Actor/CPlayer.h"
#include "Actor/CPlayerController.h"
#include "Global.h"

#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Character.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"

#include "Animation/AnimMontage.h"

#include "Field/FieldSystemObjects.h"
#include "Field/FieldSystemComponent.h"


AWeapon::AWeapon()
	:Damage(5), AdditionalDamage(0),StaminaCost(10),
	RadialFalloffMagnitude(1000000.f), RadialVectorMagnitude(15000000.f)
{
	PrimaryActorTick.bCanEverTick = true;

	UHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	
	UHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Scene);
	UHelpers::CreateComponent<UBoxComponent>(this, &WeaponCollision, "ComatCollision", Scene);
	UHelpers::CreateComponent<UAudioComponent>(this, &AudioComponent, "AttackSound", GetRootComponent());
	UHelpers::CreateComponent<UFieldSystemComponent>(this, &FieldSystemComponent, "FieldSystemComponent", GetRootComponent());

	RadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("RadialFalloff"));
	RadialVector = CreateDefaultSubobject<URadialVector>(TEXT("RadialVector"));
	MetaData = CreateDefaultSubobject<UFieldSystemMetaDataFilter>(TEXT("MetaData"));

	
	AudioComponent->SetAutoActivate(false);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	WeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::BoxBeginOverlap);
	//이거 안해줘서 자꾸터졌다
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	//이 무기를 가진 캐릭터의 컨트롤러를 등록함
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

bool AWeapon::IsSameTagWithTarget(AActor* other,const FName& tag)
{
	return Owner->ActorHasTag(tag) && other->ActorHasTag(tag);
}

void AWeapon::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	

	IgnoreActors.AddUnique(Owner);

	if (IsSameTagWithTarget(OtherActor, "Enemy")) return;
	if (IsSameTagWithTarget(OtherActor,"Player")) return;

	if (!!OtherActor && !IgnoreActors.Contains(OtherActor))
	{
		IICharacter* other = Cast<IICharacter>(OtherActor);
		
		CheckNull(other);
		if (Owner->ActorHasTag("Player"))
			AdditionalDamage = Cast<ACPlayer>(Owner)->GetStrDamage();

		IgnoreActors.AddUnique(OtherActor);
		CreateField(GetActorLocation());

		other->Hit(GetActorLocation());
		UGameplayStatics::ApplyDamage(OtherActor, Damage + AdditionalDamage, WeaponInstigator, Owner, DamageTypeClass);
	}

}
void AWeapon::CreateField(const FVector& FieldLocation)
{
	MetaData->ObjectType = EFieldObjectType::Field_Object_Destruction;
	RadialFalloff->SetRadialFalloff(RadialFalloffMagnitude, 0.8f, 1.0f, 0.f, 200.f, FieldLocation, Field_Falloff_Linear);
	RadialVector->SetRadialVector(RadialVectorMagnitude, FieldLocation);

	FieldSystemComponent->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, RadialFalloff);
	FieldSystemComponent->ApplyPhysicsField(true, EFieldPhysicsType::Field_LinearForce, MetaData, RadialVector);

	//DrawDebugSphere(GetWorld(), FieldLocation, 25.f, 12, FColor::White, true, 30.f, 0, 1.f);
}


void AWeapon::Equip()
{
	CheckTrue(bEquipped);//이미 착용중이면 함수 종료
	CheckTrue(bEquipping);//무기 꺼내는중이여도 종료

	bEquipping = true;

	OwnerCharacter->PlayAnimMontage(DrawMontage);

	SetInstigator(OwnerCharacter->GetController()); //무기 변경 시 컨틀롤러 재등록 고려
	
}

void AWeapon::UnEquip()
{
	CheckFalse(bEquipped);
	CheckTrue(bEquipping);//무기 넣는중이여도 종료

	bEquipping = true;

	OwnerCharacter->PlayAnimMontage(SheathMontage);

}

void AWeapon::ActivateCollision()
{
	if (AudioComponent->Sound)
		AudioComponent->Play();
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	IgnoreActors.Empty();
}

void AWeapon::DeactivateCollision()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
