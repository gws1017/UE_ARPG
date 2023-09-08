#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/IInteraction.h"
#include "LostExp.generated.h"

UCLASS()
class UE_ARPG_API ALostExp : public AActor, public IInteraction
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		class USphereComponent* OverlapSphere;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		class UNiagaraComponent* ItemEffect;

	UPROPERTY(VisibleDefaultsOnly, Category = "Exp")
		class ACPlayer* PlayerInstance;
	UPROPERTY(VisibleDefaultsOnly, Category = "Exp")
		int32 Exp;
	UPROPERTY(EditAnywhere)
		float HeightOffset;

public:	
	ALostExp();

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION()
		void OverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OverlapSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OnInteraction();

	void Init(int32 LostExp, const FVector& Location);
};
