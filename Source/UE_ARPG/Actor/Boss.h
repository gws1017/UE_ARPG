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
	UPROPERTY(EditDefaultsOnly, Category = "Boss | AI") //For No Weapon Enemy
		class UCapsuleComponent* LWeaponCollision;
	UPROPERTY(EditAnyWhere, Category = "Boss | AI") //For No Weapon Enemy
		class UCapsuleComponent* RWeaponCollision;
	
	UPROPERTY(EditDefaultsOnly, Category = " Boss | AI")
		class USphereComponent* AtkCSphere;
	UPROPERTY(EditDefaultsOnly, Category = "Boss | AI")
		class USphereComponent* RangedAtkSphere; 
	//CombatSphere�� ��Ȱ���ұ������� ���� ������ �������� �����Ǵµ�
	//������ ���� �÷��̾� �ν� �� �����ϱ⶧���� ������ �ٲ���������
	//��Ȱ���� ����� ���Ƽ� ���Ÿ��� ��Ÿ� üũ Sphere Collision�� �߰��ߴ�.

	UPROPERTY(VisibleDefaultsOnly, Category = "Boss | Weapon")
		float Damage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Boss | Weapon")
		float DamageC;
	UPROPERTY(VisibleDefaultsOnly, Category = "Boss | Weapon")
		TSubclassOf<class UDamageType> DamageTypeClass;
	UPROPERTY(VisibleDefaultsOnly, Category = "Boss | Weapon")
		class AController* WeaponInstigator;

	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		class UAnimMontage* AttackMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		TArray<FName> SectionList;
	//bool ������ �߰��Ͽ� ���� A ���Ұ��ΰ�, A���� B�߰�Ÿ�� �Ұ��ΰ� �߰��� �� ����
	//���Ÿ������� �߰��غ��� �̰����� CobatCollistion(MeeleCollision �̸���������)���� ũ��
	//�� radius�� ��ū RangedAttackCollision�� �߰��϶�
	//2 phase (ü�� ��������)�� �������� AttackC�� ���ϰ����� �߰��϶�
	//������ �߰��Ǹ鼭 �������� �޶����� �ִµ� �̷��� DamageType�� ����غ���

	UPROPERTY(VisibleDefaultsOnly, Category = "Boss | Particle")
		class UParticleSystem* AttackCParticle;

	UPROPERTY(VisibleDefaultsOnly, Category = "Stone")
		class AStone* Stone;
public:
	ABoss();

	

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	UFUNCTION()
	FORCEINLINE UShapeComponent* GetCollision(FString name) { return CollisionMap.Contains(name) ? CollisionMap[name] : nullptr; }

	void Begin_Collision(FString name);
	void End_Collision(FString name);


	void Begin_Attack();
	void End_Attack();

	UFUNCTION(BlueprintCallable)
		virtual void Attack() override;

	UFUNCTION(BlueprintCallable)
		void AttackC();

	void AttackThrow();
	void SpawnStone();


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
	
	UFUNCTION()
		void AttackCBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void AttackCEndnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	
	UPROPERTY(VisibleAnywhere)
		bool bDamaged;

	UPROPERTY(VisibleAnywhere)
		bool bCanAttackC;

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Debug")
		bool bAttacking;

private:
	UPROPERTY(VisibleDefaultsOnly)
	TMap<FString,UShapeComponent*> CollisionMap;

	template<typename T>
	void SetWeaponCollision(T** Collision, FVector Location, FRotator Rotation)
	{
		(*Collision)->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		(*Collision)->SetRelativeLocation(Location);
		(*Collision)->SetRelativeRotation(Rotation);

		CollisionMap.Add((*Collision)->GetName(), *Collision);
		(*Collision)->bHiddenInGame = false; //Debug
	}
};


