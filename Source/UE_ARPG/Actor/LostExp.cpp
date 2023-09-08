#include "Actor/LostExp.h"
#include "Actor/CPlayerController.h"
#include "Actor/CPlayer.h"
#include "Global.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"

ALostExp::ALostExp()
{
	UHelpers::CreateComponent<USphereComponent>(this, &OverlapSphere, "OverlapSphere", GetRootComponent());
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "StaticMesh", OverlapSphere);
	UHelpers::CreateComponent<UNiagaraComponent>(this, &ItemEffect, "ItemEffect", OverlapSphere);
}

void ALostExp::BeginPlay()
{
	Super::BeginPlay();
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &ALostExp::OverlapSphereBeginOverlap);
	OverlapSphere->OnComponentEndOverlap.AddDynamic(this, &ALostExp::OverlapSphereEndOverlap);

	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	InputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &ALostExp::OnInteraction);
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

}

void ALostExp::OverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* OverlapPlayer = Cast<ACPlayer>(OtherActor);

	if (OverlapPlayer)
	{
		PlayerInstance = OverlapPlayer;
		EnableInput(UGameplayStatics::GetPlayerController(GetWorld(),0));
		//UI띄울거면 여기에
	}
}

void ALostExp::OverlapSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPlayer* OverlapPlayer = Cast<ACPlayer>(OtherActor);

	if (OverlapPlayer)
	{
		PlayerInstance = nullptr;
		DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
}

void ALostExp::OnInteraction()
{
	PlayerInstance->IncreamentExp(Exp);
	Exp = 0;
	if (ItemEffect)
		ItemEffect->Deactivate();

	Destroy();
}

void ALostExp::Init(int32 LostExp, const FVector& Location)
{
	Exp = LostExp;
	FVector Loc = Location;
	Loc.Z += (OverlapSphere->Bounds.SphereRadius) + HeightOffset;
	SetActorLocation(Loc);
}

