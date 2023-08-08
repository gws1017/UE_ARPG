#include "Actor/LevelUpObject.h"
#include "Actor/CPlayer.h"
#include "Actor/CPlayerController.h"
#include "Actor/EnemySpawner.h"
#include "UI/LevelUpUI.h"
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

	LevelUpUI = Cast<ULevelUpUI>(CreateWidget(GetWorld(), LevelUpUIClass));

	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(),0));
	InputComponent->BindAction("Interaction",EInputEvent::IE_Pressed,this,&ALevelUpObject::OnInteraction);
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		
}

void ALevelUpObject::OverlapBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* OverlapPlayer = Cast<ACPlayer>(OtherActor);

	if (OverlapPlayer)
	{
		EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
}

void ALevelUpObject::OverlapBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPlayer* OverlapPlayer = Cast<ACPlayer>(OtherActor);

	if (OverlapPlayer)
	{
		DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
}

void ALevelUpObject::OnInteraction()
{
	Cast<ACPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0))->ToggleUI(bVisibleLevelUpUI, LevelUpUI);
	if (bVisibleLevelUpUI) 
	{
		ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		player->SetHP(player->GetMaxHP());
		player->SetStartPoint(player->GetActorLocation());
		TArray<AEnemySpawner*> SpawnerArray;
		UHelpers::FindActors<AEnemySpawner>(GetWorld(), SpawnerArray);
		for (auto spawner : SpawnerArray)
		{
			spawner->RespawnMonster();
		}
	}
	
}

