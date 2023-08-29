#include "Actor/BossTrigger.h"
#include "Actor/CPlayer.h"
#include "Actor/Enemy.h"
#include "Actor/\Weapon.h"
#include "Actor/CPlayerController.h"
#include "UI/HUDOverlay.h"

#include "Global.h"
#include "MyGameInstance.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundCue.h"

#include "MovieSceneSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "LevelSequence.h"

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

void ABossTrigger::PlayBossCutScene(ACPlayer* player)
{
	CheckNull(BossCutScene);
	DisableInput(player->GetController<APlayerController>());

	FMovieSceneSequencePlaybackSettings setting;
	ALevelSequenceActor* OutActor;
	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), BossCutScene, setting, OutActor);
	
	TArray<AActor*> StopActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AEnemy::StaticClass(), StopActors);
	
	StopActors.Add(player);
	StopActors.Add(player->GetWeapon());

	for (auto actor : StopActors)
	{
		actor->SetActorHiddenInGame(true);
		actor->SetActorTickEnabled(false);
	}
	float EndTime = static_cast<float>(SequencePlayer->GetEndTime().AsSeconds());

	SequencePlayer->Play();
	GetWorldTimerManager().SetTimer(CutSceneTimer, [this,StopActors,player]() {
			for (auto actor : StopActors)
			{
				actor->SetActorHiddenInGame(false);
				actor->SetActorTickEnabled(true);
			}
			FinishBossCutScene(player);
		}, EndTime, false);
}

void ABossTrigger::FinishBossCutScene(ACPlayer* player)
{
	UHUDOverlay* HUD = player->GetController<ACPlayerController>()->GetHUD();
	if (!!HUD)
		HUD->ShowBossHPBar();

	ShowMesh();
	UMyGameInstance* GI = GetGameInstance<UMyGameInstance>();

	if (!!GI)
	{
		GI->PlayBGM(BossBattleMusic);
	}
}

void ABossTrigger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* player = Cast<ACPlayer>(OtherActor);

	CheckNull(player);
	
	PlayBossCutScene(player);

	if (TriggerBox->OnComponentBeginOverlap.IsBound())
		TriggerBox->OnComponentBeginOverlap.Clear();
}

