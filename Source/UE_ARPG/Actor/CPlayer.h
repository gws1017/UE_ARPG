#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ICharacter.h"
#include "CPlayer.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Dead UMETA(DisplayName = "Dead"),
	EMS_MAX UMETA(DisplayName = "DefaultMAX")

};

UCLASS()
class UE_ARPG_API ACPlayer : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Status")
		float MaxHP;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		float HP;
	//스테미나와 관련되는 요소
	/* 기본 스테미나 50
	*  달리기 - 초당 1~5 소모 (테스트해보고 조정)
	*  공격 - 기본 1회에 10소모, 무기마다 다르다 무기에 스테미나 소모량 기술
		(강공격은 추가하지 않을 예정, 만약 추가한다면 기본 소모량 * 1.5배)
	*  구르기 - 10 소모
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float MaxStamina;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float Stamina;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float StaminaRegenRate;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Enums")
		EMovementStatus MovementStatus;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void UpdateStamina(float DeltaStamina);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE void SetMovementStatus(EMovementStatus Status) { MovementStatus = Status; }

	void DecrementStamina(float Amount);

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

	virtual void DeathEnd() override {};


public:

	virtual void Hit() override;
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
