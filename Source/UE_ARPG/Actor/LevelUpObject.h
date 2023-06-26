// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelUpObject.generated.h"

UCLASS()
class UE_ARPG_API ALevelUpObject : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		class UBoxComponent* OverlapBox;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* Mesh;

public:	
	ALevelUpObject();

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION()
		void OverlapBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OverlapBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
