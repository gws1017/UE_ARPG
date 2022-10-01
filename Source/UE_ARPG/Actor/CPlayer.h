#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ICharacter.h"
#include "CPlayer.generated.h"

UCLASS()
class UE_ARPG_API ACPlayer : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Status")
		float MaxHP;
	UPROPERTY(VisibleDefaultsOnly, Category = "Status")
		float HP;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCameraComponent* Camera;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void OnMoveForward(float Axis);
	void OnMoveRight(float Axis);
	void OnHorizonLock(float Axis);
	void OnVerticalLock(float Axis);

private:

	void OnRunning();
	void OffRunning();
	void ReadyWeapon();
	void OnAttack();

	void Die();

	virtual void Hit() override;
	virtual void DeathEnd() override {};

public:

	virtual bool Alive() override;

public:
	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	class AWeapon* Weapon;


public:

	FORCEINLINE class AWeapon* GetWeapon() override { return Weapon; }

};
