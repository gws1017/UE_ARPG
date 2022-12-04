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
private:
	UPROPERTY(EditDefaultsOnly, Category = "Boss | AI") //For No Weapon Enemy
		class UCapsuleComponent* LWeaponCollision;
	UPROPERTY(EditAnyWhere, Category = "Boss | AI") //For No Weapon Enemy
		class UCapsuleComponent* RWeaponCollision;
	
	UPROPERTY(EditDefaultsOnly, Category = " Boss | AI")
		class USphereComponent* AtkCSphere;
	UPROPERTY(EditDefaultsOnly, Category = "Boss | AI")
		class USphereComponent* RangedAtkSphere; 

	UPROPERTY(VisibleDefaultsOnly, Category = "Boss | Weapon")
		float Damage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Boss | Weapon")
		float DamageC;
	UPROPERTY(VisibleDefaultsOnly, Category = "Boss | Weapon")
		float DamageD;
	UPROPERTY(VisibleDefaultsOnly, Category = "Boss | Weapon")
		TSubclassOf<class UDamageType> DamageTypeClass;
	UPROPERTY(VisibleDefaultsOnly, Category = "Boss | Weapon")
		class AController* WeaponInstigator;

	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		class UAnimMontage* AttackMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		TArray<FName> SectionList;

	//�������� ���̸鼭 ���Ÿ�������� �����̰������� ���Ÿ������Ұ��� �����϶�
	//2 phase (ü�� ��������)�� �������� AttackC�� ���ϰ����� �߰��϶�
	//������ �߰��Ǹ鼭 �������� �޶����� �ִµ� �̷��� DamageType�� ����غ���

	UPROPERTY(VisibleDefaultsOnly, Category = "Boss | Particle")
		class UParticleSystem* AttackCParticle;

	UPROPERTY(VisibleDefaultsOnly, Category = "Stone")
		class AStone* Stone;

	float JumpDelayTime;

	FTimerHandle JumpDownTimer;
public:
	ABoss();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	UFUNCTION()
		FORCEINLINE UShapeComponent* GetCollision(FString name) { return CollisionMap.find(name) != CollisionMap.end() ? CollisionMap[name] : nullptr; }

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

	void AttackJump();
	void AttackDropDownBegin();
	void AttackDropDownEnd();


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

	std::map<FString, UShapeComponent*> CollisionMap;
	//TMap<FString,UShapeComponent*> CollisionMap; //���׿����ϱ�? �ϴ� std::map������
	template<typename T>
	void SetWeaponCollision(T** Collision, FVector Location, FRotator Rotation)
	{
		(*Collision)->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		(*Collision)->SetRelativeLocation(Location);
		(*Collision)->SetRelativeRotation(Rotation);

		CollisionMap[(*Collision)->GetName()] = *Collision;
		(*Collision)->bHiddenInGame = false; //Debug
	}
};


