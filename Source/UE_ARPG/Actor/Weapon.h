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

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon || Component")
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon || Component")
		class UBoxComponent* WeaponCollision;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon || Montage")
		class UAnimMontage* DrawMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon || Montage")
		class UAnimMontage* SheathMontage;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon || Socket")
		FName WeaponSocket = "SwordSocket";
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon || Socket")
		FName SheathSocket = "SheathSocket";

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		float Damage;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		float AdditionalDamage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		float StaminaCost;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		bool bEquipped;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		bool bEquipping;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon || Field")
		class UFieldSystemComponent* FieldSystemComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon || Field")
		class URadialFalloff* RadialFalloff;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon || Field")
		class URadialVector* RadialVector;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon || Field")
		class UFieldSystemMetaDataFilter* MetaData;
	UPROPERTY(EditAnywhere, Category = "Weapon || Field")
		float RadialFalloffMagnitude;
	UPROPERTY(EditAnywhere, Category = "Weapon || Field")
		float RadialVectorMagnitude;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		TArray<AActor*> IgnoreActors;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		TSubclassOf<class UDamageType> DamageTypeClass;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		class AController* WeaponInstigator;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		class ACharacter* OwnerCharacter;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon || Sound")
		class UAudioComponent* AudioComponent;
public:	
	AWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Equip();
	virtual void UnEquip();

	virtual void Begin_Equip();
	virtual void End_Equip();

	virtual void Begin_UnEquip();
	virtual void End_UnEquip();

	virtual void Begin_Collision();
	virtual void End_Collision();

	void ActivateCollision();
	void DeactivateCollision();

	UFUNCTION()
		void CreateField(const FVector& FieldLocation);

protected:

	bool IsSameTagWithTarget(AActor* other, const FName& tag);
public:
	UFUNCTION()
		void BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	FORCEINLINE bool GetEquipped() { return bEquipped; }
	FORCEINLINE bool GetEquipping() { return bEquipping; }

	FORCEINLINE float GetDamage() { return Damage; }
	FORCEINLINE float GetStaminaCost() { return StaminaCost; }

	FORCEINLINE class UBoxComponent* GetWeaponCollision() { return WeaponCollision; }

	FORCEINLINE void SetInstigator(AController* Inst) { WeaponInstigator = Inst; }

};
