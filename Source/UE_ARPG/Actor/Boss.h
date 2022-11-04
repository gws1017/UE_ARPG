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
	//CombatSphere를 재활용할까했지만 공격 패턴이 무작위로 결정되는데
	//범위에 들어온 플레이어 인식 후 공격하기때문에 구조를 바꾸지않으면
	//재활용은 힘들것 같아서 원거리용 사거리 체크 Sphere Collision을 추가했다.

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
	//bool 변수를 추가하여 공격 A 만할것인가, A이후 B추가타를 할것인가 추가할 수 있음
	//원거리공격을 추가해보자 이공격은 CobatCollistion(MeeleCollision 이름변경하자)보다 크다
	//즉 radius가 더큰 RangedAttackCollision을 추가하라
	//2 phase (체력 절반이하)로 떨어지면 AttackC와 낙하공격을 추가하라
	//공격이 추가되면서 데미지가 달라지고 있는데 이러면 DamageType을 사용해보자

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


