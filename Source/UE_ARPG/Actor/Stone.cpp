#include "Actor/Stone.h"
#include "Actor/Boss.h"
#include "Actor/CPlayer.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
//#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"

AStone::AStone()
{
	PrimaryActorTick.bCanEverTick = true;

	//UHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh");

	UStaticMesh* mesh;
	UMaterialInstanceConstant* material;
	UHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Engine/BasicShapes/Sphere.Sphere'");
	UHelpers::GetAsset<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Enemy/BossBear/Stone/Material/BasicShapeMaterial_Inst.BasicShapeMaterial_Inst'");
	UHelpers::GetAsset<UParticleSystem>(&ImpactParticle, "ParticleSystem'/Game/Enemy/BossBear/Stone/FX/p_RipplingSmash_SegmentFX.p_RipplingSmash_SegmentFX'");
	UHelpers::GetAsset<UMaterialInstanceConstant>(&DecalMaterial, "MaterialInstanceConstant'/Game/Enemy/BossBear/Stone/Material/M_Decal_Inst.M_Decal_Inst'");

	/*Projectile->InitialSpeed = 1.5e+3f;
	Projectile->MaxSpeed = 1.5e+3f;
	Projectile->ProjectileGravityScale = 1.0f;
	Projectile->bSimulationEnabled = false;

	Projectile->SetUpdatedComponent(Mesh);*/

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetStaticMesh(mesh);
	Mesh->SetMaterial(0, material);
	Mesh->SetRelativeScale3D(FVector(0.5));


	InitialLifeSpan = 3.f;
	Damage = 5;
	Speed = 1e+3f;

}

void AStone::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh->OnComponentHit.AddDynamic(this, &AStone::OnHit);

}

void AStone::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ABoss* boss = Cast<ABoss>(OtherActor);
	if (boss) return;

	if (!!OtherActor) 
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mesh->SetSimulatePhysics(false);

		ACPlayer* player = Cast<ACPlayer>(OtherActor);


		if (!!player)
		{
			player->Hit();
			UGameplayStatics::ApplyDamage(player, Damage,NULL, this, DamageTypeClass);
		}

		FTransform trans(NormalImpulse.Rotation(), HitComponent->GetRelativeLocation(), FVector(1));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, trans);
		if(!player) UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMaterial, FVector(100), HitComponent->GetRelativeLocation(), NormalImpulse.Rotation());
		Destroy();
	}
}

void AStone::Throw(ACharacter* Causer, const FVector& NewVelocity)
{
	
	Causer->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetPhysicsLinearVelocity(NewVelocity * Speed);

	/*Projectile->bSimulationEnabled = true;
	Projectile->Velocity = NewVelocity * Projectile->InitialSpeed;*/

}

void AStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



