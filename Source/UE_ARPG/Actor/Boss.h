// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
private:
	UPROPERTY(EditDefaultsOnly, Category = "AI") //For No Weapon Enemy
		class UCapsuleComponent* LWeaponCollision;
	UPROPERTY(EditAnyWhere, Category = "AI") //For No Weapon Enemy
		class UCapsuleComponent* RWeaponCollision;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		float Damage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		TSubclassOf<class UDamageType> DamageTypeClass;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		class AController* WeaponInstigator;

	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		class UAnimMontage* AttackMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		TArray<FName> SectionList;
	//bool 변수를 추가하여 공격 A 만할것인가, A이후 B추가타를 할것인가 추가할 수 있음
	//원거리공격을 추가해보자 이공격은 CobatCollistion(MeeleCollision 이름변경하자)보다 크다
	//즉 radius가 더큰 RangedAttackCollision을 추가하라
	//2 phase (체력 절반이하)로 떨어지면 AttackC와 낙하공격을 추가하라

public:
	ABoss();

protected:
	virtual void BeginPlay() override;

public:
	class UCapsuleComponent* GetCollsion(FString name) { return CollisionMap[name]; }

	void Begin_Collision(FString name);
	void End_Collision(FString name);

	void Begin_Attack();
	void End_Attack();

	UFUNCTION(BlueprintCallable)
		virtual void Attack() override;


public:
	UFUNCTION()
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	UFUNCTION()
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	UFUNCTION()
		virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void AttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	bool bAttacking;
	UPROPERTY(VisibleAnywhere)
	bool bDamaged;


private:
	UPROPERTY(VisibleDefaultsOnly)
	TMap<FString,UCapsuleComponent*> CollisionMap;
};
