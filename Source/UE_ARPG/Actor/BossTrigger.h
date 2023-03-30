#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossTrigger.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API ABossTrigger : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Trigger")
		class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleDefaultsOnly,Category = "Sound")
	class USoundCue* BossBattleMusic;

	UPROPERTY(EditAnywhere, Category = "Sound")
	class UStaticMeshComponent* WallMesh;
public:
		ABossTrigger();


		virtual void BeginPlay() override;
public:
		void HideMesh();
		void ShowMesh();

public:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
