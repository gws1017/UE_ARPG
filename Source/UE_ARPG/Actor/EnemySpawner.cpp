#include "Actor/EnemySpawner.h"
#include "Actor/Enemy.h"
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
	AEnemy* actor = GetWorld()->SpawnActor<AEnemy>(Monster,GetActorLocation(), GetActorRotation(), param);
	if (actor)
	{
		actor->SetSpawner(this);
		MonsterCount++;
	}
	
}

void AEnemySpawner::RespawnMonster()
{
	if (MonsterCount == 0)
	{
		CLog::Print("Respawn");
		SpawnMonster();
	}
}

