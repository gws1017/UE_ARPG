#include "Actor/Boss.h"
#include "Actor/CPlayer.h"
#include "Actor/Stone.h"
#include "AI/Controller/EnemyBTController.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraComponent.h"

ABoss::ABoss() :
	Damage (5.f), DamageC (7.f), DamageD (9.f)
	, JumpDelayTime(2.5f), DropLocationOffset(200.f) , SectionNumber(0)
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

	UHelpers::GetAsset<UParticleSystem>(&AttackParticle, "ParticleSystem'/Game/Enemy/BossBear/Effect/ExplosionParticle/FX/p_RipplingSmash_SegmentFX.p_RipplingSmash_SegmentFX'");
	UHelpers::CreateComponent<UNiagaraComponent>(this, &NSParticleComponent, "NSComponent", GetRootComponent());
	UNiagaraSystem* NSAsset;
	UHelpers::GetAsset<UNiagaraSystem>(&NSAsset, "NiagaraSystem'/Game/sA_Megapack_v1/sA_StylizedAttacksPack/FX/NiagaraSystems/NS_AOE_Attack_1.NS_AOE_Attack_1'");
	NSParticleComponent->SetAsset(NSAsset);
	UHelpers::SocketAttachComponent<UCapsuleComponent>(this, &LWeaponCollision, "LWeaponCollision", GetMesh(), "hand_lf");
	UHelpers::SocketAttachComponent<UCapsuleComponent>(this, &RWeaponCollision, "RWeaponCollision", GetMesh(), "hand_rt");
	UHelpers::CreateComponent<USphereComponent>(this, &JumpAtkSphere, "JumpAtkSphere", GetMesh());
	UHelpers::CreateComponent<USphereComponent>(this, &RangedAtkSphere, "RangedAtkSphere", GetRootComponent());

	WeaponInstigator = GetController();

	GetCapsuleComponent()->SetHiddenInGame(false);
	GetCapsuleComponent()->SetCapsuleHalfHeight(150.f);
	GetCapsuleComponent()->SetCapsuleRadius(130.f); RWeaponCollision->SetCapsuleHalfHeight(60);

	RWeaponCollision->SetCapsuleRadius(40);
	LWeaponCollision->SetCapsuleHalfHeight(60);
	LWeaponCollision->SetCapsuleRadius(40);

	SetWeaponCollision(&RWeaponCollision, FVector(-8, -16, -1.5), FRotator(-80, 0, -30));
	SetWeaponCollision(&LWeaponCollision, FVector(8, 16, 1.5), FRotator(-80, 0, -30));
	SetWeaponCollision(&JumpAtkSphere, FVector(0,0,0), FRotator::ZeroRotator);
	JumpAtkSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	RangedAtkSphere->InitSphereRadius(350.f);
	JumpAtkSphere->InitSphereRadius(200.f);
	AgroSphere->InitSphereRadius(500.f);
	CombatSphere->InitSphereRadius(170.f);

	MaxHP = 50;
	HP = 30;
	
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
	RangedAtkSphere->OnComponentBeginOverlap.AddDynamic(this, &ABoss::CombatSphereOnOverlapBegin);
	RangedAtkSphere->OnComponentEndOverlap.AddDynamic(this, &ABoss::CombatSphereOnOverlapEnd);
	JumpAtkSphere->OnComponentBeginOverlap.AddDynamic(this, &ABoss::CombatSphereOnOverlapBegin);
	JumpAtkSphere->OnComponentEndOverlap.AddDynamic(this, &ABoss::CombatSphereOnOverlapEnd);
	
}

void ABoss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

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

bool ABoss::IsHitActorRangedAttack(const FVector& start, const FVector& end, float radius,TArray<AActor*>& HitActors)
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


	CheckFalseResult(result,result);

	for (auto hitresult : HitResults)
		HitActors.AddUnique(hitresult.GetActor());

	return result;
}

void ABoss::AttackC()
{
	FVector ActorLocation2D = GetActorLocation();
	ActorLocation2D.Z = 0;
	FVector start = ActorLocation2D + GetActorForwardVector() * 100.f;
	FVector end = start + GetActorForwardVector() * 100.f;
	FVector TargetLocation = FVector::ZeroVector;

	TArray<AActor*> HitActors;

	if (IsHitActorRangedAttack(start, end, 100.f, HitActors)) 
	{
		for (auto HitActor : HitActors)
		{
			ACPlayer* player = Cast<ACPlayer>(HitActor);
			if (!!player)
			{
				if (player->IsHit() == false)
					UGameplayStatics::ApplyDamage(player, DamageC, WeaponInstigator, this, DamageTypeClass);
				TargetLocation = player->GetActorLocation();
			}
		}
	}
	
	FRotator rot = UKismetMathLibrary::FindLookAtRotation(TargetLocation, start);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackParticle, FTransform(rot, start, FVector(1, 1, 1)));
	NSParticleComponent->SetWorldLocation(end);
	NSParticleComponent->Activate();
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

	if (bRangedAtkJump)
	{
		FVector target_location = CombatTarget->GetActorLocation();
		target_location += CombatTarget->GetActorForwardVector() * DropLocationOffset;
		target_location.Z = GetActorLocation().Z;
		SetActorLocation(target_location);
		//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}

}

void ABoss::AttackDropDownEnd()
{
	CheckTrue(bDamaged);
	bDamaged = true;

	FVector start = FVector(GetActorLocation().X, GetActorLocation().Y, 0.f);
	FVector end = start + GetActorForwardVector() * 100.f;

	TArray<AActor*> HitActors;

	if (IsHitActorRangedAttack(start, end, 200.f, HitActors))
	{
		for (auto HitActor : HitActors)
		{
			ACPlayer* player = Cast<ACPlayer>(HitActor);
			if (!!player)
			{
				UGameplayStatics::ApplyDamage(player, DamageD, WeaponInstigator, this, DamageTypeClass);
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
	CheckFalse(bRanged);
	AEnemy::Attack();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!!AnimInstance)
	{
		AnimInstance->Montage_Play(AttackMontage);
		
		SelectAttack(SectionNumber);
		
		AnimInstance->Montage_JumpToSection(SectionList[SectionNumber]);
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

void ABoss::SelectAttack(int32& num)
{
	switch (BossPhase)
	{
	case 1:
		AttackNumber++;
		if (AttackNumber > SectionList.Num() - 1) AttackNumber = 0;
		num = FMath::RandRange(0, 2);
		break;
	case 2:
		num = FMath::RandRange(0, SectionList.Num()-1);
		break;
	}

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
		}
	}
}

void ABoss::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!!OtherActor && Alive())
	{
		CheckNull(CombatTarget); //목표가 없으면 종료
		ACPlayer* player = Cast<ACPlayer>(OtherActor);
		if (!!player)
		{
			bRangedAtkJump = true;
			bRanged = true;
		}
	}
}

void ABoss::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!!OtherActor && Alive())
	{
		bRanged = false;
		bRangedAtkJump = false;
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
			//피격 이펙트 및 사운드 추가부분
			//사운드는 무기에서 얻고 피격 이펙트는 맞는 대상에서 가져온다
			UGameplayStatics::ApplyDamage(player, Damage, WeaponInstigator, this, DamageTypeClass);
			bDamaged = true;
		}
		
	}
}
