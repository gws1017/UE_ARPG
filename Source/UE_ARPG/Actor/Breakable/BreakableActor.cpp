#include "Actor/Breakable/BreakableActor.h"
#include "Interface/ICharacter.h"
#include "Actor/Weapon.h"
#include "Global.h"

#include "Components/AudioComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	UHelpers::CreateComponent<UGeometryCollectionComponent>(this, &GeometryCollectionComponent, "GeoMetryCollectionComponent");
	UHelpers::CreateComponent<UAudioComponent>(this, &AudioComponent, "AudioComponent", GeometryCollectionComponent);

	AudioComponent->SetAutoActivate(false);
	GeometryCollectionComponent->SetGenerateOverlapEvents(true);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	//GeometryCollectionComponent->SetRelativeScale3D(FVector(UKismetMathLibrary::RandomFloatInRange(0.7, 1.0)));

}

void ABreakableActor::Hit(const FVector& ParticleSpawnLocation)
{
	if (AudioComponent->Sound)
		AudioComponent->Play();
}



