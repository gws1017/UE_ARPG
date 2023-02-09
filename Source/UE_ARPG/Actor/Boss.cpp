#include "Actor/Boss.h"
#include "Actor/CPlayer.h"
#include "Actor/Stone.h"
#include "AI/Controller/EnemyBTController.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystem.h"

ABoss::ABoss() :
	Damage (5.f), DamageC (7.f), DamageD (9.f)
	, JumpDelayTime(2.5f), DropLocationOffset(200.f)
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

	UHelpers::GetAsset<UParticleSystem>(&AttackCParticle, "ParticleSystem'/Game/Enemy/BossBear/FX/p_RipplingSmash_SegmentFX.p_RipplingSmash_SegmentFX'");

	UHelpers::SocketAttachComponent<UCapsuleComponent>(this, &LWeaponCollision, "LWeaponCollision", GetMesh(), "hand_lf");
	UHelpers::SocketAttachComponent<UCapsuleComponent>(this, &RWeaponCollision, "RWeaponCollision", GetMesh(), "hand_rt");
	UHelpers::CreateComponent<USphereComponent>(this, &AtkCSphere, "AtkCSphere",GetMesh());
	UHelpers::CreateComponent<USphereComponent>(this, &JumpAtkSphere, "JumpAtkSphere", GetMesh());
	UHelpers::CreateComponent<USphereComponent>(this, &RangedAtkSphere, "RangedAtkSphere", GetRootComponent());

	WeaponInstigator = GetController();

	GetCapsuleComponent()->SetCapsuleHalfHeight(150.f);
	GetCapsuleComponent()->SetCapsuleRadius(130.f); RWeaponCollision->SetCapsuleHalfHeight(60);

	RWeaponCollision->SetCapsuleRadius(40);
	LWeaponCollision->SetCapsuleHalfHeight(60);
	LWeaponCollision->SetCapsuleRadius(40);

	SetWeaponCollision(&RWeaponCollision, FVector(-8, -16, -1.5), FRotator(-80, 0, -30));
	SetWeaponCollision(&LWeaponCollision, FVector(8, 16, 1.5), FRotator(-80, 0, -30));
	SetWeaponCollision(&AtkCSphere, FVector(0, 155, -3), FRotator(0, 0, 0));
	SetWeaponCollision(&JumpAtkSphere, FVector(0,0,0), FRotator::ZeroRotator);
	AtkCSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	JumpAtkSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	AtkCSphere->InitSphereRadius(80.f);
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
	AtkCSphere->OnComponentBeginOverlap.AddDynamic(this, &ABoss::AttackCBeginOverlap);
	AtkCSphere->OnComponentEndOverlap.AddDynamic(this, &ABoss::AttackCEndnOverlap);
	RangedAtkSphere->OnComponentBeginOverlap.AddDynamic(this, &ABoss::CombatSphereOnOverlapBegin);
	RangedAtkSphere->OnComponentEndOverlap.AddDynamic(this, &ABoss::CombatSphereOnOverlapEnd);
	JumpAtkSphere->OnComponentBeginOverlap.AddDynamic(this, &ABoss::CombatSphereOnOverlapBegin);
	JumpAtkSphere->OnComponentEndOverlap.AddDynamic(this, &ABoss::CombatSphereOnOverlapEnd);
	
	/*MaxHP = 50;
	HP = 10;*/
	/*HP = 20;
	SectionList.Add("AttackThrow");
	SectionList.Add("AttackJump");*/
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

void ABoss::AttackC()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	EObjectTypeQuery Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(Pawn);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this); //자기자신은 충돌검사 X

	TArray<FHitResult> HitResults;

	FVector ActorLocation2D = GetActorLocation();
	ActorLocation2D.Z = 0;
	FVector start = ActorLocation2D + GetActorForwardVector() * 100.f;
	FVector end = start + GetActorForwardVector() * 100.f;
	FVector TargetLocation = FVector::ZeroVector;

	bool result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), start, end, 100.f,
		ObjectTypes, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResults, true);

	TArray<AActor*> UniqueHitActors;

	CheckFalse(result);
	for (auto hitresult : HitResults)
		UniqueHitActors.AddUnique(hitresult.GetActor());

	for (auto hitresult : UniqueHitActors)
	{
		//CLog::Log(hitresult->GetName());
		ACPlayer* player = Cast<ACPlayer>(hitresult);
		if (!!player)
		{
			if (player->IsHit() == false)
				UGameplayStatics::ApplyDamage(player, DamageC, WeaponInstigator, this, DamageTypeClass);
			player->Hit();
			TargetLocation = player->GetActorLocation();
		}
	}

	FRotator rot = UKismetMathLibrary::FindLookAtRotation(TargetLocation, start);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackCParticle, FTransform(rot, start, FVector(1, 1, 1)));

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

	TimerCallback.BindLambda([this, anim]()
	{
		anim->Montage_Play(AttackMontage);
		anim->Montage_JumpToSection("AttackDown", AttackMontage);
		GetMesh()->SetHiddenInGame(true);
	});

	GetWorldTimerManager().SetTimer(JumpDownTimer, TimerCallback, UKismetMathLibrary::RandomFloatInRange(1.0f, JumpDelayTime),false);
}

void ABoss::AttackDropDownBegin()
{
	GetMesh()->SetHiddenInGame(false);

	CheckNull(CombatTarget);

	if (bRangedAtkJump)
	{
		FVector target_location = CombatTarget->GetActorLocation();
		target_location += CombatTarget->GetActorForwardVector() * DropLocationOffset;
		target_location.Z = 0;
		SetActorLocation(target_location);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}

}

void ABoss::AttackDropDownEnd()
{
	CheckNull(CombatTarget);

	if (bRangedAtkJump && bDamaged == false)
	{
		bDamaged = true;
		//CLog::Print("player attack drop down");
		CombatTarget->Hit();
		UGameplayStatics::ApplyDamage(CombatTarget, DamageD, WeaponInstigator, this, DamageTypeClass);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackCParticle, GetActorLocation(), FRotator::ZeroRotator);
	}
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

}

void ABoss::Begin_Attack()
{
}

void ABoss::End_Attack()
{
	bAttacking = false;
	bDamaged = false;
	//CLog::Print("End_ ATTACK");
}

void ABoss::Attack()
{
	CheckFalse(bRanged);
	AEnemy::Attack();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!!AnimInstance)
	{
		AnimInstance->Montage_Play(AttackMontage);
		int32 num;
		
		SelectAttack(num);

		if (SectionList[num] == "AttackThrow") {
			CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		}
		else CombatSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		
		AnimInstance->Montage_JumpToSection(SectionList[num]);
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
		num = 2;//FMath::RandRange(0, 2);
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
			//목표를 발견하면 목표에게 다가갈것인가?
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

			//사거리밖이면 이동하자?
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
			player->Hit();
			UGameplayStatics::ApplyDamage(player, Damage, WeaponInstigator, this, DamageTypeClass);
			bDamaged = true;
		}
		
	}
}

//플레이어가 범위안에있는지 항상 확인한다.
void ABoss::AttackCBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!!OtherActor)
	{
		ACPlayer* player = Cast<ACPlayer>(OtherActor);
		if (!!player)
		{
			bCanAttackC = true;
		}
	}
}

void ABoss::AttackCEndnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!!OtherActor)
	{

		ACPlayer* player = Cast<ACPlayer>(OtherActor);
		if (!!player)
		{
			bCanAttackC = false;
		}
	}
}
