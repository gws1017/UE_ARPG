#include "Actor/Enemy.h"
#include "Actor/Weapon.h"
#include "Actor/CPlayer.h"
#include "AI/Controller/EnemyController.h"
#include "Actor/EnemySpawner.h"
#include "Global.h"

#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystem.h"
#include "AIController.h"

AEnemy::AEnemy()
	: MaxHP(1),HP(1),Exp(1)
{
	UHelpers::CreateComponent<USphereComponent>(this,&AgroSphere, "AgroSphere",GetRootComponent());
	AgroSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	UHelpers::CreateComponent<USphereComponent>(this, &CombatSphere, "CombatSphere", GetRootComponent());
	
	UHelpers::CreateComponent<UAudioComponent>(this, &AudioComponent, "AttackSound", GetRootComponent());

	AudioComponent->SetAutoActivate(false);

	AIControllerClass = AEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	WeaponInstigator = GetController();

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AEnemyController>(GetController());

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	SpawnLocation = GetActorLocation();
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount <= 0.f)
		return DamageAmount;

	if (HP - DamageAmount <= 0.f) //체력이 0이될때 적용후 Die함수 호출
	{
		HP = FMath::Clamp(HP - DamageAmount, 0.0f, MaxHP);
		CombatTarget->IncreamentExp(Exp);
		CombatTarget->SetTarget(nullptr);
		Die();
	}
	else //일반적인 데미지 계산
	{
		HP = FMath::Clamp(HP - DamageAmount, 0.0f, MaxHP);
	}
	UE_LOG(LogTemp, Display, L"Enemy Current HP : %f", HP);
	return DamageAmount;
}

void AEnemy::TargetApplyDamage(ACPlayer* player, float damage, const FVector& HitLocation)
{
	CheckNull(player);
	player->Hit(HitLocation);
	UGameplayStatics::ApplyDamage(player, damage, WeaponInstigator, this, DamageTypeClass);
}

bool AEnemy::IsHitActorAreaAttack(const FVector& start, const FVector& end, float radius, TArray<AActor*>& HitActors)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	EObjectTypeQuery Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(Pawn);

	TArray<AActor*> IgnoreActors;
	//자기자신은 충돌검사 X
	IgnoreActors.Add(this);

	TArray<FHitResult> HitResults;
	bool result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), start, end, radius,
		ObjectTypes, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResults, true);


	CheckFalseResult(result, result);

	for (auto hitresult : HitResults)
		HitActors.AddUnique(hitresult.GetActor());

	return result;
}

bool AEnemy::IsRanged(float radius)
{
	FVector start = GetActorLocation() - FVector(radius,0,0);
	FVector end = GetActorLocation() + FVector(radius,0,0);

	TArray<AActor*> HitActors;

	if(IsHitActorAreaAttack(start,end,radius, HitActors))
		return true;

	return false;
}

FVector AEnemy::GetCombatTargetLocation() const
{
	CheckNullResult(CombatTarget,GetActorForwardVector());
	return CombatTarget->GetActorLocation();
}

void AEnemy::SetAttackTimer()
{
	float AttackTime = FMath::FRandRange(2.0, 4.0);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime,true);

}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::Attack()
{
	CheckFalse(Alive());
	CheckNull(CombatTarget);
	CheckFalse(CombatTarget->Alive());
	CheckFalse(bAlerted);
	CheckTrue(bAttacking);
	
	bAttacking = true;

	if (AIController)
	{
		AIController->StopMovement();
	}
}
	

void AEnemy::Hit(const FVector& ParticleSpawnLocation)
{
	if (AudioComponent->Sound)
		AudioComponent->Play();
	if(HitParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, ParticleSpawnLocation, FRotator(0.f), false);
	PlayAnimMontage(HitMontage);
}

void AEnemy::Die()
{
	if(Spawner)
		Spawner->DecrementMonsterCount();
	if (CombatTarget)
	{
		CombatTarget->SetTarget(NULL);
		CombatTarget = nullptr;
	}
	
	StopAnimMontage();
	PlayAnimMontage(DeathMontage);

	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if(!!Weapon)
		Weapon->DeactivateCollision();
}

void AEnemy::Disappear()
{
	//사라지면서 해야할 것들 작성
	if(!!Weapon)
		Weapon->Destroy();

	Destroy();
}

void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay);
}

void AEnemy::MoveToTarget(ACharacter* Target)
{
	CheckNull(Target);
	CheckFalse(Alive());
	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(10.0f);

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);

	}
	else CLog::Log("Null AI Controller");;
}

void AEnemy::MoveToSpawnLocation()
{
	CheckFalse(Alive());
	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalLocation(SpawnLocation);
		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);
		
		GetWorldTimerManager().SetTimer(AlertTimer, this, &AEnemy::AlertEnd, AlertDuration);
	}
}

void AEnemy::AlertEnd()
{
	SetAlerted(false);
}

bool AEnemy::Alive()
{
	if (FMath::IsNearlyZero(HP)) return false;
	else return true;
}


void AEnemy::Begin_Attack()
{
}

void AEnemy::End_Attack()
{
	//Notify로 호출
	bAttacking = false;
	SetAttackTimer();
}
