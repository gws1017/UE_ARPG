#include "Actor/Boss.h"
#include "Actor/CPlayer.h"
#include "Actor/Stone.h"
#include "Actor/CPlayerController.h"
#include "UI/HUDOverlay.h"
#include "AI/Controller/EnemyBTController.h"
#include "MyGameInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraComponent.h"

ABoss::ABoss() :
	Damage (5.f), DamageC (7.f), DamageD (9.f)
	, JumpDelayTime(2.5f), DropLocationOffset(200.f) , AttackNumber(0)
	, SectionList{ "AttackA","AttackB","AttackC","AttackThrow","AttackJump" }
{
	USkeletalMesh* mesh;
	UHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Enemy/BossBear/Mesh/Boss_Bear.Boss_Bear'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -140));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TSubclassOf<UAnimInstance> anim;
	UHelpers::GetClass(&anim, "AnimBlueprint'/Game/Enemy/BossBear/Animation/ABP_Bear.ABP_Bear_C'");
	GetMesh()->SetAnimInstanceClass(anim);

	UHelpers::GetAsset<UAnimMontage>(&AttackMontage, "AnimMontage'/Game/Enemy/BossBear/Montage/Bear_Attack_Montage.Bear_Attack_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&DeathMontage, "AnimMontage'/Game/Enemy/BossBear/Montage/Bear_Death_Montage.Bear_Death_Montage'");

	UHelpers::CreateComponent<UNiagaraComponent>(this, &NSParticleComponent, "NSComponent", GetRootComponent());

	UNiagaraSystem* NSAsset;
	UHelpers::GetAsset<UParticleSystem>(&AttackParticle, "ParticleSystem'/Game/Enemy/BossBear/Effect/ExplosionParticle/FX/p_RipplingSmash_SegmentFX.p_RipplingSmash_SegmentFX'");
	UHelpers::GetAsset<UNiagaraSystem>(&NSAsset, "NiagaraSystem'/Game/Enemy/BossBear/Effect/CrackParticle/FX/NiagaraSystems/NS_AOE_Attack_1.NS_AOE_Attack_1'");
	NSParticleComponent->SetAsset(NSAsset);
	NSParticleComponent->SetAutoActivate(false);
	
	UHelpers::SocketAttachComponent<UCapsuleComponent>(this, &LWeaponCollision, "LWeaponCollision", GetMesh(), "hand_lf");
	UHelpers::SocketAttachComponent<UCapsuleComponent>(this, &RWeaponCollision, "RWeaponCollision", GetMesh(), "hand_rt");

	GetCapsuleComponent()->SetCapsuleHalfHeight(150.f);
	GetCapsuleComponent()->SetCapsuleRadius(130.f); RWeaponCollision->SetCapsuleHalfHeight(60);

	RWeaponCollision->SetCapsuleRadius(40);
	LWeaponCollision->SetCapsuleHalfHeight(60);
	LWeaponCollision->SetCapsuleRadius(40);

	SetWeaponCollision(&RWeaponCollision, FVector(-8, -16, -1.5), FRotator(-80, 0, -30));
	SetWeaponCollision(&LWeaponCollision, FVector(8, 16, 1.5), FRotator(-80, 0, -30));

	AgroSphere->InitSphereRadius(1400.f);

	GetCharacterMovement()->MaxWalkSpeed = 300;

	MaxHP = 50;
	HP = 50;
	
	AIControllerClass = AEnemyBTController::StaticClass();
	//BossPhase = 1;
	
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();
	
	auto SetCollision = [this](UCapsuleComponent** Collision)
	{
		(*Collision)->OnComponentBeginOverlap.AddDynamic(this, &ABoss::AttackBeginOverlap);
		(*Collision)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		(*Collision)->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		(*Collision)->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		(*Collision)->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	};

	SetCollision(&LWeaponCollision);
	SetCollision(&RWeaponCollision);
	
}

void ABoss::Begin_Collision(FString name)
{
	GetCollision(name)->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABoss::End_Collision(FString name)
{
	GetCollision(name)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bDamaged = false;

}


void ABoss::AttackC()
{
	FVector ActorLocation = GetActorLocation();
	FVector start = ActorLocation + GetActorForwardVector() * 100.f;
	FVector end = start + GetActorForwardVector() * 100.f;
	start.Z -= 140;
	end.Z -= 140;
	FVector TargetLocation = FVector::ZeroVector;

	TArray<AActor*> HitActors;

	if (IsHitActorAreaAttack(start, end, 100.f, HitActors))
	{
		for (auto HitActor : HitActors)
		{
			ACPlayer* player = Cast<ACPlayer>(HitActor);
			if (!!player)
			{
				if (player->IsHit() == false)
				{

					TargetApplyDamage(player,DamageC);
				}
				TargetLocation = player->GetActorLocation();
			}
		}
	}
	
	FRotator rot = UKismetMathLibrary::FindLookAtRotation(TargetLocation, start);
	FTransform trans = FTransform(rot, start, FVector(1.f));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackParticle, trans);
	NSParticleComponent->SetWorldLocation(end);
	NSParticleComponent->Activate(true);
}

void ABoss::AttackThrow()
{
	CheckNull(Stone);
	FVector dir;
	if (!!CombatTarget) dir = UKismetMathLibrary::GetDirectionUnitVector(Stone->GetActorLocation(), CombatTarget->GetActorLocation());
	else dir = GetActorForwardVector();
	dir.Normalize();

	Stone->Throw(this,dir);
	
}

void ABoss::SpawnStone()
{
	FActorSpawnParameters params;
	params.Owner = this;
	Stone = GetWorld()->SpawnActor<AStone>(params);
	Stone->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "ThrowSocket");

}

void ABoss::AttackJump()
{
	GetMesh()->SetHiddenInGame(true);
	UAnimInstance* anim = GetMesh()->GetAnimInstance();
	FTimerDelegate TimerCallback;
	GetMesh()->bPauseAnims = true;
	TimerCallback.BindLambda([this, anim]()
	{
		GetMesh()->bPauseAnims = false;
		anim->Montage_Play(AttackMontage);
		anim->Montage_JumpToSection("AttackDown", AttackMontage);
	});

	GetWorldTimerManager().SetTimer(JumpDownTimer, TimerCallback, UKismetMathLibrary::RandomFloatInRange(1.0f, JumpDelayTime),false);
}

void ABoss::AttackDropDownBegin()
{
	GetMesh()->SetHiddenInGame(false);
	
	FVector target_location = CombatTarget->GetActorLocation();
	target_location += CombatTarget->GetActorForwardVector() * DropLocationOffset;
	target_location.Z = GetActorLocation().Z;
	SetActorLocation(target_location);

}

void ABoss::AttackDropDownEnd()
{
	CheckTrue(bDamaged);
	bDamaged = true;

	FVector start = FVector(GetActorLocation().X, GetActorLocation().Y, 0.f);
	FVector end = start + GetActorForwardVector() * 100.f;

	TArray<AActor*> HitActors;

	if (IsHitActorAreaAttack(start, end, 200.f, HitActors))
	{
		for (auto HitActor : HitActors)
		{
			ACPlayer* player = Cast<ACPlayer>(HitActor);
			if (!!player)
			{
				TargetApplyDamage(player, DamageD);
			}

		}
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackParticle, GetActorLocation(), FRotator::ZeroRotator);

}

void ABoss::Begin_Attack()
{
	bAttacking = true;
}

void ABoss::End_Attack()
{
	bAttacking = false;
	bDamaged = false;
}

void ABoss::Attack()
{
	CheckNull(CombatTarget);
	AEnemy::Attack();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!!AnimInstance)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(SectionList[AttackNumber]);
	}
}

void ABoss::CalculateBossPhase()
{
	int32 HpPersent = (int32)(HP / MaxHP * 100.f);
	if (HpPersent >= 50)
	{
		BossPhase = 1;
	}
	else if (HpPersent <50)
	{
		BossPhase = 2;
	}
}

void ABoss::SelectAttack()
{
	switch (BossPhase)
	{
	case 1:
		AttackNumber = FMath::RandRange(0, 2);
		break;
	case 2:
		AttackNumber = FMath::RandRange(0, SectionList.Num()-1);
		break;
	}

}

void ABoss::Die()
{
	UHUDOverlay* HUD = CombatTarget->GetController<ACPlayerController>()->GetHUD();
	HUD->HideBossHPBar();
	GetGameInstance<UMyGameInstance>()->PlayMainBGM();
	AEnemy::Die();

}

void ABoss::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!!OtherActor && Alive())
	{
		ACPlayer* player = Cast<ACPlayer>(OtherActor);
		if (!!player)
		{
			SetAlerted(true);
			CombatTarget = player;
			player->SetTarget(this);
		}
	}
}

void ABoss::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!!OtherActor && Alive())
	{
		ACPlayer* player = Cast<ACPlayer>(OtherActor);
		if (!!player)
		{
			CombatTarget = nullptr;
			bRanged = false;
			player->SetTarget(nullptr);
		}
	}
}

//Attack A & B
void ABoss::AttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (!!OtherActor)
	{
		ACPlayer* player = Cast<ACPlayer>(OtherActor);
		if (!!player && bDamaged == false)
		{
			TargetApplyDamage(player, Damage);

			bDamaged = true;
		}
		
	}
}
