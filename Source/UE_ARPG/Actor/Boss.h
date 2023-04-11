// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Global.h"

#include "CoreMinimal.h"
#include "Actor/Enemy.h"
#include "Boss.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API ABoss : public AEnemy
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Boss | AI") //For No Weapon Enemy
		class UCapsuleComponent* LWeaponCollision;
	UPROPERTY(EditAnyWhere, Category = "Boss | AI") //For No Weapon Enemy
		class UCapsuleComponent* RWeaponCollision;

	UPROPERTY(VisibleDefaultsOnly, Category = "Boss | Weapon")
		float Damage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Boss | Weapon")
		float DamageC;
	UPROPERTY(VisibleDefaultsOnly, Category = "Boss | Weapon")
		float DamageD;
	UPROPERTY(VisibleDefaultsOnly, Category = "Boss | Weapon")
		float JumpDelayTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boss | Weapon")
		float DropLocationOffset;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Boss | Status")
		int32 BossPhase = 1;

	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		class UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
		int32 AttackNumber;
	UPROPERTY(VisibleAnywhere, Category = "Animation")
		TArray<FName> SectionList;

	//공격이 추가되면서 데미지가 달라지고 있는데 이러면 DamageType을 사용해보자

	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "Boss | Particle")
		class UNiagaraComponent* NSParticleComponent;
	UPROPERTY(EditAnywhere, Category = "Boss | Particle")
		class UParticleSystem* AttackParticle;

	UPROPERTY(VisibleDefaultsOnly, Category = "Stone")
		class AStone* Stone;

	FTimerHandle JumpDownTimer;

public:
	ABoss();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		FORCEINLINE UShapeComponent* GetCollision(FString name) { return CollisionMap.find(name) != CollisionMap.end() ? CollisionMap[name] : nullptr; }

	void Begin_Collision(FString name);
	void End_Collision(FString name);


	virtual void Begin_Attack() override;
	virtual void End_Attack() override;

	UFUNCTION(BlueprintCallable)
		virtual void Attack() override;

	UFUNCTION(BlueprintCallable)
		void AttackC();


	void AttackThrow();
	void SpawnStone();

	void AttackJump();
	void AttackDropDownBegin();
	void AttackDropDownEnd();

	UFUNCTION(BlueprintCallable)
		void CalculateBossPhase();

	void SelectAttack();

	virtual void Die() override;
public:
	UFUNCTION()
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	UFUNCTION()
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	UFUNCTION()
		void AttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	
	UPROPERTY(VisibleAnywhere)
		bool bDamaged;
	UPROPERTY(VisibleAnywhere)
		bool bCanAttackC;
	UPROPERTY(VisibleAnywhere)
		bool bRangedAtkJump;

public:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Debug")
		bool bAttacking;


public:
		UFUNCTION(BlueprintPure)
			FORCEINLINE float GetBossPhase() { return BossPhase; }

		UFUNCTION(BlueprintPure)
			FORCEINLINE float GetAttackNumber() { return AttackNumber; }
private:

	std::map<FString, UShapeComponent*> CollisionMap;
	//TMap<FString,UShapeComponent*> CollisionMap;
	template<typename T>
	void SetWeaponCollision(T** Collision, FVector Location, FRotator Rotation)
	{
		(*Collision)->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		(*Collision)->SetRelativeLocation(Location);
		(*Collision)->SetRelativeRotation(Rotation);

		CollisionMap[(*Collision)->GetName()] = *Collision;
		//(*Collision)->bHiddenInGame = false; //Debug
	}
};


