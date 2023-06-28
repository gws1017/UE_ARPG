#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/IInteraction.h"
#include "LevelUpObject.generated.h"

UCLASS()
class UE_ARPG_API ALevelUpObject : public AActor , public IInteraction
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		class UBoxComponent* OverlapBox;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* Mesh;

public:

	UPROPERTY(BlueprintReadOnly, Category = "UI")
		class ULevelUpUI* LevelUpUI;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		class TSubclassOf<ULevelUpUI> LevelUpUIClass;

	bool bVisibleLevelUpUI = false;

public:	
	ALevelUpObject();

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION()
		void OverlapBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OverlapBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void OnInteraction() override;
};
