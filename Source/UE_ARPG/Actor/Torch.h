#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Torch.generated.h"

UCLASS()
class UE_ARPG_API ATorch : public AActor
{
	GENERATED_BODY()
	
private:

	
	UPROPERTY(VIsibleAnywhere)
	UStaticMeshComponent* mesh;

	
public:
	UPROPERTY(VIsibleAnywhere)
		class UPointLightComponent* FireLight;

	UPROPERTY(VIsibleAnywhere)
		class UPointLightComponent* PointLight;
public:	
	// Sets default values for this actor's properties
	ATorch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
