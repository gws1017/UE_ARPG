#include "Actor/BossTrigger.h"
#include "Actor/CPlayer.h"
#include "Actor/CPlayerController.h"
#include "UI/HUDOverlay.h"

#include "Global.h"
#include "MyGameInstance.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundCue.h"

ABossTrigger::ABossTrigger()
{
	UStaticMesh* WallAsset;
	UMaterialInstanceConstant* material;
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &WallMesh, "Wall");
	UHelpers::CreateComponent<UBoxComponent>(this, &TriggerBox, "TriggerBox", WallMesh);

	UHelpers::GetAsset<UStaticMesh>(&WallAsset, "StaticMesh'/Game/Dungeon/DugeonWall/wall_low2.wall_low2'");
	UHelpers::GetAsset<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Dungeon/DugeonWall/MI_Wall.MI_Wall'");
	UHelpers::GetAsset<USoundCue>(&BossBattleMusic, "SoundCue'/Game/Audio/BossBattleSoundCue.BossBattleSoundCue'");

	WallMesh->SetStaticMesh(WallAsset);
	WallMesh->SetMaterial(0, material);
	TriggerBox->SetRelativeLocation(FVector(80, -15, -140));
	TriggerBox->SetRelativeScale3D(FVector(1, 7, 1));
	HideMesh();
}

void ABossTrigger::HideMesh()
{
	WallMesh->SetVisibility(false);
	WallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABossTrigger::ShowMesh()
{
	WallMesh->SetVisibility(true);
	WallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ABossTrigger::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABossTrigger::OnComponentBeginOverlap);
}

void ABossTrigger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* player = Cast<ACPlayer>(OtherActor);

	CheckNull(player);
	
	UHUDOverlay* HUD = player->GetController<ACPlayerController>()->GetHUD();
	if(!!HUD)
		HUD->ShowBossHPBar();

	ShowMesh();
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());

	if (!!GI)
	{
		GI->PlayBGM(BossBattleMusic);
	}
	TriggerBox->OnComponentBeginOverlap.Clear();
}
