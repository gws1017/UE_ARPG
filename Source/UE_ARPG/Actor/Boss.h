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
	//bool ������ �߰��Ͽ� ���� A ���Ұ��ΰ�, A���� B�߰�Ÿ�� �Ұ��ΰ� �߰��� �� ����
	//���Ÿ������� �߰��غ��� �̰����� CobatCollistion(MeeleCollision �̸���������)���� ũ��
	//�� radius�� ��ū RangedAttackCollision�� �߰��϶�
	//2 phase (ü�� ��������)�� �������� AttackC�� ���ϰ����� �߰��϶�

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
