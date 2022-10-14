#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class UE_ARPG_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	template<typename T>
	static AWeapon* Spawn(class UWorld* InWorld, class ACharacter* InOwner)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;
		
		return InWorld->SpawnActor<T>(params);
		
	}

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene; 

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		class UBoxComponent* WeaponCollision;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		class UAnimMontage* DrawMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		class UAnimMontage* SheathMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		class UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		FName WeaponSocket = "SwordSocket";
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		FName SheathSocket = "SheathSocket";

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		float Damage;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		TSubclassOf<class UDamageType> DamageTypeClass;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		class AController* WeaponInstigator;
public:	
	AWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Equip();
	virtual void UnEquip();
	virtual void Attack();

	virtual void Begin_Equip();
	virtual void End_Equip();

	virtual void Begin_UnEquip();
	virtual void End_UnEquip();

	virtual void Begin_Attack();
	virtual void End_Attack();

	virtual void Begin_Collision();
	virtual void End_Collision();

	void ActivateCollision();
	void DeactivateCollision();

public:
	UFUNCTION()
		void ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	FORCEINLINE bool GetEquipped() { return bEquipped; }
	FORCEINLINE bool GetEquipping() { return bEquipping; }
	FORCEINLINE bool GetAttacking() { return bAttacking; }

	FORCEINLINE float GetDamage() { return Damage; }

	FORCEINLINE void SetInstigator(AController* Inst) { WeaponInstigator = Inst; }

protected:
	class ACharacter* OwnerCharacter;

	bool bEquipped;
	bool bEquipping;
	bool bAttacking;

};
