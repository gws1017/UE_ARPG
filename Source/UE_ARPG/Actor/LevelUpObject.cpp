#include "Actor/LevelUpObject.h"
#include "Actor/CPlayer.h"
#include "Actor/CPlayerController.h"
#include "Global.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


ALevelUpObject::ALevelUpObject()
{
	UHelpers::CreateComponent<UBoxComponent>(this, &OverlapBox, "OverlapBox", GetRootComponent());
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "StaticMesh", OverlapBox);
}

void ALevelUpObject::BeginPlay()
{
	Super::BeginPlay();
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelUpObject::OverlapBoxBeginOverlap);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &ALevelUpObject::OverlapBoxEndOverlap);
}

void ALevelUpObject::OverlapBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* OverlapPlayer = Cast<ACPlayer>(OtherActor);

	if (OverlapPlayer)
	{
		OverlapPlayer->GetController<ACPlayerController>()->SetReadyInteraction(true);
	}
}

void ALevelUpObject::OverlapBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPlayer* OverlapPlayer = Cast<ACPlayer>(OtherActor);

	if (OverlapPlayer)
	{
		OverlapPlayer->GetController<ACPlayerController>()->SetReadyInteraction(false);
	}
}

