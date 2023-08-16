// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ICharacter.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class UE_ARPG_API ABreakableActor : public AActor , public IICharacter
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleDefaultsOnly)
		UGeometryCollectionComponent* GeometryCollectionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Sound")
		class UAudioComponent* AudioComponent;

public:	
	ABreakableActor();

protected:
	virtual void BeginPlay() override;


	// 인터페이스 분리할것
	virtual class AWeapon* GetWeapon() override { return nullptr; }

	virtual bool Alive() override { return false; }

	virtual void Hit(const FVector& ParticleSpawnLocation) override;

	virtual void DeathEnd() override {}

	virtual void Begin_Attack() override {}

	virtual void End_Attack() override {}

};
