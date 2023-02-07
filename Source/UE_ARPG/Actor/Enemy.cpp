#include "Actor/Enemy.h"
#include "Actor/Weapon.h"
#include "Actor/CPlayer.h"
#include "AI/Controller/EnemyController.h"
#include "Global.h"

#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "AIController.h"

AEnemy::AEnemy()
	: MaxHP(1),HP(1)
{
	PrimaryActorTick.bCanEverTick = true;
	UHelpers::CreateComponent<USphereComponent>(this,&AgroSphere, "AgroSphere",GetRootComponent());
	AgroSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	UHelpers::CreateComponent<USphereComponent>(this, &CombatSphere, "CombatSphere", GetRootComponent());
	
	AIControllerClass = AEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		Die();
	}
	else //일반적인 데미지 계산
	{
		HP = FMath::Clamp(HP - DamageAmount, 0.0f, MaxHP);
	}
	UE_LOG(LogTemp, Display, L"Enemy Current HP : %f", HP);
	return DamageAmount;
}

 FVector AEnemy::GetTargetLocation() const
{
	CheckNullResult(CombatTarget,GetActorForwardVector());
	return CombatTarget->GetActorLocation();
}

void AEnemy::SetAttackTimer()
{
	float AttackTime = FMath::FRandRange(0.5, 2.5);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);

}

void AEnemy::Attack()
{
	CheckFalse(Alive());
	CheckFalse(bAlerted);
	CheckTrue(bAttacking);
	CheckNull(CombatTarget);
	CheckFalse(CombatTarget->Alive());

	bAttacking = true;

	if (AIController)
	{
		AIController->StopMovement();
	}
}
	

void AEnemy::Hit()
{
	PlayAnimMontage(HitMontage);
}

void AEnemy::Die()
{
	PlayAnimMontage(DeathMontage);

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

void AEnemy::MoveToTarget(ACharacter* Target)
{
	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(10.0f);

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);

	}
}

void AEnemy::MoveToSpawnLocation()
{
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
	if (HP <= 0) return false;
	else return true;
}

void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay);
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
