// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Enemy.h"
#include "EnemySkeletonMelee.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API AEnemySkeletonMelee : public AEnemy
{
	GENERATED_BODY()
	
public:
	AEnemySkeletonMelee();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Animation")
		class UAnimMontage* AttackMontage;
protected:
	virtual void BeginPlay() override;

public:
		virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
		virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
		virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
		virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:

	virtual void Attack() override;
};
