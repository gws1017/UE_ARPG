#include "Actor/Torch.h"

#include "Global.h"

#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"

ATorch::ATorch()
{
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &mesh, "Mesh", GetRootComponent());
	
	UStaticMesh* Asset;
	UMaterialInstanceConstant* material;
	UHelpers::GetAsset<UStaticMesh>(&Asset, "StaticMesh'/Game/Dungeon/Dugeon_torch/torch_low.torch_low'");
	UHelpers::GetAsset<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Dungeon/Dugeon_torch/MI_Torch.MI_Torch'");


	mesh->SetStaticMesh(Asset);
	mesh->SetMaterial(0, material);
	UHelpers::CreateComponent<UPointLightComponent>(this, &FireLight, "Fire", GetRootComponent());
	FireLight->SetRelativeLocation(FVector(-10, 0, 40));
	FireLight->SetLightColor(FLinearColor(1.0, 0.33, 0.17));
	FireLight->SetAttenuationRadius(30.f);
	FireLight->SetMobility(EComponentMobility::Stationary);

	UHelpers::CreateComponent<UPointLightComponent>(this, &PointLight, "Point", GetRootComponent());
	PointLight->SetRelativeLocation(FVector(-120, 0, 0));
	PointLight->SetLightColor(FLinearColor(1.0, 0.33, 0.17));
	PointLight->SetAttenuationRadius(255.f);
	PointLight->SetMobility(EComponentMobility::Stationary);

}

// Called when the game starts or when spawned
void ATorch::BeginPlay()
{
	Super::BeginPlay();
	
}

