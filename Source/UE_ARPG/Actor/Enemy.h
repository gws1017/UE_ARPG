// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ICharacter.h"
#include "Enemy.generated.h"

UCLASS()
class UE_ARPG_API AEnemy : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Status")
	float MaxHP;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Status")
	float HP;
	UPROPERTY(VisibleDefaultsOnly, Category = "Status")
	FVector SpawnLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
		class ACPlayer* CombatTarget;
	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
		class AAIController* AIController;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "AI")
		class USphereComponent* AgroSphere;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class USphereComponent* CombatSphere;
	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
		bool bAlerted;
	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
		bool bRanged;

	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		class UAnimMontage* DeathMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		class UAnimMontage* HitMontage;

	FTimerHandle AttackTimer;
	FTimerHandle AlertTimer;
	float AlertDuration = 3.0f;
	FTimerHandle DeathTimer;
	float DeathDelay = 3.0f;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {};
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {};
	
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {};
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {};


	//AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject
public:

	FORCEINLINE void SetAlerted(bool value) { bAlerted = value; }
	FORCEINLINE bool GetAlerted() { return bAlerted; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsRanged() { return bRanged; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE ACPlayer* GetTarget() { return (!!CombatTarget) ? CombatTarget : nullptr; }

public:

	void SetAttackTimer();
	virtual void Attack();
	void Hit();
	void Die();
	void Disappear();

	void MoveToTarget(ACharacter* Target);
	void MoveToSpawnLocation();
	
	void AlertEnd();

	virtual bool Alive() override;
	virtual void DeathEnd() override;
protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		class AWeapon* Weapon;

public:

	FORCEINLINE class AWeapon* GetWeapon() override { return Weapon; }
};
