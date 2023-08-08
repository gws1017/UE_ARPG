#include "Actor/LostExp.h"
#include "Actor/CPlayerController.h"
#include "Actor/CPlayer.h"
#include "Global.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"

ALostExp::ALostExp()
{
	UHelpers::CreateComponent<UBoxComponent>(this, &OverlapBox, "OverlapBox", GetRootComponent());
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "StaticMesh", OverlapBox);

	UStaticMesh* mesh;
	UHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Engine/EngineMeshes/Sphere.Sphere'");
	UMaterialInstanceConstant* material;
	UHelpers::GetAsset<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Engine/BasicShapes/BasicShapeMaterial_Inst.BasicShapeMaterial_Inst'");

	Mesh->SetStaticMesh(mesh);
	Mesh->SetMaterial(0, material);

	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void ALostExp::BeginPlay()
{
	Super::BeginPlay();
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ALostExp::OverlapBoxBeginOverlap);

	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	InputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &ALostExp::OnInteraction);
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

}

void ALostExp::OverlapBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* OverlapPlayer = Cast<ACPlayer>(OtherActor);

	if (OverlapPlayer)
	{
		PlayerInstance = OverlapPlayer;
		EnableInput(UGameplayStatics::GetPlayerController(GetWorld(),0));
		//UI띄울거면 여기에
	}
}

void ALostExp::OverlapBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
	Mesh->DestroyComponent();
	OverlapBox->DestroyComponent();
	Destroy();
}

void ALostExp::Init(int32 LostExp, const FVector& Location)
{
	Exp = LostExp;
	FVector Loc = Location;
	Loc.Z += (Mesh->GetStaticMesh()->GetBounds().GetBox().GetExtent() 
		* Mesh->GetRelativeScale3D()).Z;
	SetActorLocation(Loc);
}

