#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stone.generated.h"

UCLASS()
class UE_ARPG_API AStone : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Stone")
		class UStaticMeshComponent* Mesh;

	/*UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;*/

	UPROPERTY(VisibleDefaultsOnly, Category = "Stone")
		class UParticleSystem* ImpactParticle;

	UPROPERTY(VisibleDefaultsOnly, Category = "Stone")
		class UMaterialInstanceConstant* DecalMaterial;

	UPROPERTY(VisibleDefaultsOnly, Category = "Stone")
		TSubclassOf<class UDamageType> DamageTypeClass;
	UPROPERTY(VisibleDefaultsOnly, Category = "Stone")
		float Damage;

	UPROPERTY(VisibleDefaultsOnly, Category = "Stone")
		float Speed;

public:	
	AStone();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	virtual void Tick(float DeltaTime) override;

private:

	class ACharacter* OwnerCharacter;

public:
	UFUNCTION()
		void Throw(ACharacter* Causer, const FVector& NewVelocity);
};
