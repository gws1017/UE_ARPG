#include "Actor/EnemySpawner.h"
#include "Global.h"

#include "Components/BoxComponent.h"

AEnemySpawner::AEnemySpawner()
{
	UHelpers::CreateComponent<UBoxComponent>(this, &SpawnBox, "SpawnBox", GetRootComponent());

}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnMonster();
}

void AEnemySpawner::SpawnMonster()
{
	FActorSpawnParameters param;
	verifyf(Monster,L"Actor class invalid");
	AActor* actor = GetWorld()->SpawnActor<AActor>(Monster,GetActorLocation(), GetActorRotation(), param);
	SpawnArray.Add(actor);

}

void AEnemySpawner::RespawnMonster()
{
	if (SpawnArray.Num() == 0)
	{
		CLog::Print("Respawn");
		SpawnMonster();
	}
}

