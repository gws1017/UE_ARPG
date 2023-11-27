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
enum class EPlayerStat : uint8
{
	EPS_Normal UMETA(DisplayName = "Normal"),
	EPS_Invincible UMETA(DisplayName = "Invincible"),
	EPS_MAX UMETA(DisplayName = "DefaultMAX")

};

USTRUCT(BlueprintType)
struct FPlayerStatus
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere, Category = "Status")
		float HP;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		float MaxHP;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		float Stamina;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		float MaxStamina;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		float StrengthDamage;

	UPROPERTY(VisibleAnywhere, Category = "Status")
		int32 Vigor; //����� HP�� ������ ��ħ
	UPROPERTY(VisibleAnywhere, Category = "Status")
		int32 Enduarance; //������ ���׹̳��� ������ ��ħ
	UPROPERTY(VisibleAnywhere, Category = "Status")
		int32 Strength; //�� ���� �������� ������ ��ħ
	UPROPERTY(VisibleAnywhere, Category = "Status")
		int32 Level;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		int32 Exp;

};

UCLASS()
class UE_ARPG_API ACPlayer : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Status")
		FPlayerStatus Stat;

	UPROPERTY(VisibleDefaultsOnly)
		FVector StartPoint;
	UPROPERTY(VisibleDefaultsOnly)
		FVector DeathLocation;
	//���׹̳��� ���õǴ� ���
	/* �⺻ ���׹̳� 50
	*  �޸��� - �ʴ� 1~5 �Ҹ� (�׽�Ʈ�غ��� ����)
	*  ���� - �⺻ 1ȸ�� 10�Ҹ�, ���⸶�� �ٸ��� ���⿡ ���׹̳� �Ҹ� ���
		(�������� �߰����� ���� ����, ���� �߰��Ѵٸ� �⺻ �Ҹ� * 1.5��)
	*  ������ - 10 �Ҹ�
	*/
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
		class UAudioComponent* AudioComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Combat")
		class AEnemy* Target;

	UPROPERTY(VisibleAnywhere, Category = "Enums")
		EMovementState MovementState;
	UPROPERTY(VisibleAnywhere, Category = "Enums")
		EPlayerStat PlayerStat;
	
	UPROPERTY(VisibleAnywhere)
		bool bHit = false;

	UPROPERTY(VisibleAnywhere, Category = "Debug")
		bool bSaveAttack = false;
	UPROPERTY(VisibleAnywhere, Category = "Debug")
		bool bAttacking = false;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		class AWeapon* Weapon;
	UPROPERTY(VisibleDefaultsOnly, Category = "Controller")
		class ACPlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "BP Class")
		TSubclassOf<class ALostExp> LostExpClass;

	bool bRunning = false;

	bool bBlending = false;

protected:
	//ĳ���� �⺻�Լ�
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//Ű�Է� ���� �Լ�
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
	//���� ���� �Լ�
	
	virtual bool Alive() override;
	void Die();
	virtual void DeathEnd();

	virtual void Hit(const FVector& ParticleSpawnLocation) override;
	virtual void HitEnd();

	bool CanAttack();
	bool CanRoll();
	bool CanMove();

	virtual void Begin_Attack()override;
	virtual void End_Attack()override;
	void ComboAttackSave();
	void ResetCombo();
	void PlayAttackMontage();

	void UpdateStamina(float DeltaStamina);

	void LevelUp(const FPlayerStatus& data);

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION()
		void DecrementStamina(float Amount);

	//��Ÿ �Լ�
	UFUNCTION(BlueprintCallable)
		void SaveGameData(int32 SaveType = 0);
	UFUNCTION(BlueprintCallable)
		void LoadGameData();
public:
	//Getter & Setter
	FORCEINLINE void SetPlayerState(EPlayerStat state) { PlayerStat = state; }
	FORCEINLINE void SetMovementState(EMovementState state) { MovementState = state; }
	FORCEINLINE void SetTarget(class AEnemy* target) { Target = target; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class AWeapon* GetWeapon() override { return Weapon; }
	FORCEINLINE class AEnemy* GetTarget() { return Target; }

	FORCEINLINE FVector GetStartPoint() { return StartPoint; }
	FORCEINLINE void SetStartPoint(const FVector& vec) { StartPoint = vec; }

	UFUNCTION(BlueprintPure)
		float GetDamage();
	FORCEINLINE	float GetStrDamage() { return Stat.StrengthDamage; }
	FORCEINLINE	void SetStrDamage(float dmg) { Stat.StrengthDamage = dmg; }
	FORCEINLINE float GetHP() { return Stat.HP; }
	FORCEINLINE float GetMaxHP() { return Stat.MaxHP; }
	FORCEINLINE void SetMaxHP(float hp) { Stat.MaxHP = hp; }
	FORCEINLINE void SetHP(float hp) { Stat.HP = hp; }
	
	FORCEINLINE float GetStamina() { return Stat.Stamina; }
	FORCEINLINE float GetMaxStamina() { return Stat.MaxStamina; }
	FORCEINLINE void SetMaxStamina(float sta) { Stat.MaxStamina = sta; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetPlayerLevel() { return Stat.Level; }
	FORCEINLINE void SetPlayerLevel(int32 lev) { Stat.Level = lev; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetExp() { return Stat.Exp; }
	UFUNCTION()
		FORCEINLINE void SetExp(int32 e) { Stat.Exp = e; }
	UFUNCTION()
		void IncreamentExp(int32 e);

	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetVigor() { return Stat.Vigor; }
	FORCEINLINE void SetVigor(int32 vig) { Stat.Vigor = vig; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetEnduarance() { return Stat.Enduarance; }
	FORCEINLINE void SetEnduarance(int32 end) { Stat.Enduarance = end; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetStrength() { return Stat.Strength; }
	FORCEINLINE void SetStrength(int32 str) { Stat.Strength = str; }

	FORCEINLINE bool IsValidTarget() { return (!!Target); }
	FORCEINLINE bool IsHit() { return bHit; }
	FORCEINLINE bool IsInvincible() { return PlayerStat == EPlayerStat::EPS_Invincible; }
	FORCEINLINE void SetBlending(bool value) { bBlending = value; }
	FORCEINLINE bool IsBlending() { return bBlending; }
};
