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
	EMS_Hit UMETA(DisplayName = "Hit"),
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

	//���׹̳��� ���õǴ� ���
	/* �⺻ ���׹̳� 50
	*  �޸��� - �ʴ� 1~5 �Ҹ� (�׽�Ʈ�غ��� ����)
	*  ���� - �⺻ 1ȸ�� 10�Ҹ�, ���⸶�� �ٸ��� ���⿡ ���׹̳� �Ҹ� ���
		(�������� �߰����� ���� ����, ���� �߰��Ѵٸ� �⺻ �Ҹ� * 1.5��)
	*  ������ - 10 �Ҹ�
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float MaxStamina;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float Stamina;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float StaminaRegenRate;
	UPROPERTY()
		class UHUDOverlay* PlayerHUDOverlay; 
	UPROPERTY(BlueprintReadOnly, Category = "UI")
		class TSubclassOf<UHUDOverlay> HUDOverlayClass;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		class UAnimMontage* DeathMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		class UAnimMontage* HitMontage;

	UPROPERTY(VisibleAnywhere, Category = "Enums")
		EMovementStatus MovementStatus;

	UPROPERTY(EditAnywhere, Category = "Animation")
		bool bHitMovement = false;
	
	UPROPERTY(VisibleAnywhere)
		bool bHit = false;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void UpdateStamina(float DeltaStamina);

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

public:

	virtual void Hit() override;
	virtual bool Alive() override;

	virtual void DeathEnd();
	virtual void HitEnd();

	bool CanAttack();

public:
	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION()
		void DecrementStamina(float Amount);
private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	class AWeapon* Weapon;


public:

	FORCEINLINE void SetMovementStatus(EMovementStatus Status) { MovementStatus = Status; }

	FORCEINLINE class AWeapon* GetWeapon() override { return Weapon; }

	FORCEINLINE float GetHP() { return HP; }
	FORCEINLINE float GetMaxHP() { return MaxHP; }
	FORCEINLINE float GetStamina() { return Stamina; }
	FORCEINLINE float GetMaxStamina() { return MaxStamina; }
};
