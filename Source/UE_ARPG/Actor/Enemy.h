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

	UPROPERTY(VisibleDefaultsOnly, Category = "Status")
	float MaxHP;
	UPROPERTY(VisibleDefaultsOnly, Category = "Status")
	float HP;

	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		class UAnimMontage* DeathMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		class UAnimMontage* HitMontage;

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

public:

	void Hit();
	void Die();
	void Disappear();
	
	
	virtual void DeathEnd() override;
protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		class AWeapon* Weapon;

public:

	FORCEINLINE class AWeapon* GetWeapon() override { return Weapon; }
};
