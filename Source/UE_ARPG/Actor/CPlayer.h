#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ICharacter.h"
#include "CPlayer.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Hit UMETA(DisplayName = "Hit"),
	EMS_Dead UMETA(DisplayName = "Dead"),
	EMS_Roll UMETA(DisplayName = "Roll"),
	EMS_MAX UMETA(DisplayName = "DefaultMAX")

};

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	EPS_Normal UMETA(DisplayName = "Normal"),
	EPS_Invincible UMETA(DisplayName = "Invincible"),
	EPS_MAX UMETA(DisplayName = "DefaultMAX")

};

UCLASS()
class UE_ARPG_API ACPlayer : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Status")
		int32 Level;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		int32 Exp;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		int32 MaxExp;
	//생명력 HP에 영향을 줌
	UPROPERTY(VisibleAnywhere, Category = "Status")
		int32 Vigor;
	//지구력 스테미나에 영향을줌
	UPROPERTY(VisibleAnywhere, Category = "Status")
		int32 Enduarance;
	//힘 최종데미지에 영향을줌
	UPROPERTY(VisibleAnywhere, Category = "Status")
		int32 Strength;

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

	float RollStamina;

	UPROPERTY(VisibleAnywhere)
		int32 AttackCount;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		class UAnimMontage* DeathMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		class UAnimMontage* HitMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		class UAnimMontage* Attack1Montage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		class UAnimMontage* Attack2Montage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		class UAnimMontage* RollMontage;

	UPROPERTY(VisibleDefaultsOnly, Category = "Sound")
		class USoundCue* AttackSoundCue;
	UPROPERTY(VisibleDefaultsOnly, Category = "Sound")
		class UAudioComponent* AttackAudioComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Combat")
		class AEnemy* Target;

	UPROPERTY(VisibleAnywhere, Category = "Enums")
		EMovementState MovementState;
	UPROPERTY(VisibleAnywhere, Category = "Enums")
		EPlayerState PlayerStat;
	
	UPROPERTY(VisibleAnywhere)
		bool bHit = false;

	UPROPERTY(VisibleAnywhere, Category = "Debug")
		bool bSaveAttack = false;
	UPROPERTY(VisibleAnywhere, Category = "Debug")
		bool bAttacking = false;

	bool bRunning = false;

	bool bBlending = false;

	TMap<FString, float> StaminaTable;

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

	void OnRunning();
	void OffRunning();
	void ReadyWeapon();
	void OnAttack();
	void OnRoll();
	void ESCDown();
	void ESCUp();


public:
	void Die();

	virtual void Hit(const FVector& ParticleSpawnLocation) override;
	virtual bool Alive() override;

	virtual void DeathEnd();
	virtual void HitEnd();

	bool CanAttack();
	bool CanRoll();

	virtual void Begin_Attack()override;
	virtual void End_Attack()override;

	void PlayAttackMontage();
	void ResetCombo();
	void ComboAttackSave();

public:
	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION()
		void DecrementStamina(float Amount);
	UFUNCTION()
		void WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	class AWeapon* Weapon;


public:

	FORCEINLINE void SetPlayerState(EPlayerState state) { PlayerStat = state; }
	FORCEINLINE void SetMovementState(EMovementState state) { MovementState = state; }
	FORCEINLINE void SetTarget(class AEnemy* target) { Target = target; }

	FORCEINLINE class AWeapon* GetWeapon() override { return Weapon; }
	FORCEINLINE class AEnemy* GetTarget() { return Target; }

	FORCEINLINE float GetHP() { return HP; }
	FORCEINLINE float GetMaxHP() { return MaxHP; }
	FORCEINLINE float GetStamina() { return Stamina; }
	FORCEINLINE float GetMaxStamina() { return MaxStamina; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetPlayerLevel() { return Level; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetExp() { return Exp; }
	UFUNCTION()
		FORCEINLINE void SetExp(int32 e) { Exp = e; }
	UFUNCTION()
		void IncreamentExp(int32 e);
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetMaxExp() { return MaxExp; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetVigor() { return Vigor; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetEnduarance() { return Enduarance; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetStrength() { return Strength; }

	FORCEINLINE bool IsValidTarget() { return (!!Target); }
	FORCEINLINE bool IsHit() { return bHit; }
	FORCEINLINE bool IsInvincible() { return PlayerStat == EPlayerState::EPS_Invincible; }
	FORCEINLINE void SetBlending(bool value) { bBlending = value; }
	FORCEINLINE bool IsBlending() { return bBlending; }
};
