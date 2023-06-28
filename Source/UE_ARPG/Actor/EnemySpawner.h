// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class UE_ARPG_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UBoxComponent* SpawnBox;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AActor> Monster;

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
		TArray<class AActor*> SpawnArray;


public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
		void SpawnMonster();
	UFUNCTION()
		void RespawnMonster();
};
