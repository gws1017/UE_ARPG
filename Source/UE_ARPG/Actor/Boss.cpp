#include "Actor/Boss.h"
#include "Actor/CPlayer.h"
#include "Global.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystem.h"

ABoss::ABoss()
{
	USkeletalMesh* mesh;
	UHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Enemy/BossBear/Mesh/Boss_Bear.Boss_Bear'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -140));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	GetCapsuleComponent()->SetCapsuleHalfHeight(150.f);
	GetCapsuleComponent()->SetCapsuleRadius(130.f);

	TSubclassOf<UAnimInstance> anim;
	UHelpers::GetClass(&anim, "AnimBlueprint'/Game/Enemy/BossBear/Animation/ABP_Bear.ABP_Bear_C'");
	GetMesh()->SetAnimInstanceClass(anim);

	UHelpers::GetAsset<UParticleSystem>(&AttackCParticle, "ParticleSystem'/Game/Enemy/BossBear/FX/p_RipplingSmash_SegmentFX.p_RipplingSmash_SegmentFX'");


	UHelpers::SocketAttachComponent<UCapsuleComponent>(this, &LWeaponCollision, "LWeaponCollision", GetMesh(), "hand_lf");
	SetWeaponCollision(&LWeaponCollision, FVector(8, 16, 1.5), FRotator(-80, 0, -30));
	LWeaponCollision->SetCapsuleHalfHeight(60);
	LWeaponCollision->SetCapsuleRadius(40);
	UHelpers::SocketAttachComponent<UCapsuleComponent>(this, &RWeaponCollision, "RWeaponCollision", GetMesh(), "hand_rt");
	SetWeaponCollision(&RWeaponCollision, FVector(-8, -16, -1.5), FRotator(-80, 0, -30));
	RWeaponCollision->SetCapsuleHalfHeight(60);
	RWeaponCollision->SetCapsuleRadius(40);
	UHelpers::CreateComponent<USphereComponent>(this, &AtkCCollision, "AttackCCollision",GetMesh());
	SetWeaponCollision(&AtkCCollision, FVector(0,155,-3), FRotator(0, 0, 0));
	AtkCCollision->InitSphereRadius(80.f);
	AtkCCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	WeaponInstigator = GetController();
	Damage = 5;
	DamageC = 7;

	UHelpers::GetAsset<UAnimMontage>(&AttackMontage, "AnimMontage'/Game/Enemy/BossBear/Montage/Bear_Attack_Montage.Bear_Attack_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&DeathMontage, "AnimMontage'/Game/Enemy/BossBear/Montage/Bear_Death_Montage.Bear_Death_Montage'");

	SectionList.Add("AttackA");
	SectionList.Add("AttackB");
	SectionList.Add("AttackC");

	MaxHP = 50;
	HP = MaxHP;

	AgroSphere->InitSphereRadius(300.f);
	CombatSphere->InitSphereRadius(170.f);
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
	AtkCCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoss::AttacCkBeginOverlap);
	AtkCCollision->OnComponentEndOverlap.AddDynamic(this, &ABoss::AttacCkEndnOverlap);

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

//플레이어가 범위안에있을때 한번만 처리
void ABoss::AttackC()
{
	FVector start = GetActorForwardVector();
	FVector loc = GetActorLocation();
	loc.Z = 0;

	if (!!CombatTarget) {
		FVector target = CombatTarget->GetActorLocation();
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(start, target);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackCParticle, FTransform(rot, loc, FVector(1, 1, 1)));

		CheckFalse(bCanAttackC);
		CombatTarget->Hit();
		UGameplayStatics::ApplyDamage(CombatTarget, DamageC, WeaponInstigator, this, DamageTypeClass);
	}
	else {
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(FVector::Zero(), start);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackCParticle, FTransform(rot, loc, FVector(1, 1, 1)));
	}
	

}

void ABoss::Begin_Attack()
{
}

void ABoss::End_Attack()
{
	bAttacking = false;
}

void ABoss::Attack()
{
	CheckNull(CombatTarget);
	CheckFalse(CombatTarget->Alive());
	CheckTrue(bAttacking);
	CheckFalse(bRanged);

	AEnemy::Attack();

	bAttacking = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!!AnimInstance)
	{
		AnimInstance->Montage_Play(AttackMontage);
		int32 num = FMath::FRandRange(0, 2);
		AnimInstance->Montage_JumpToSection(SectionList[2]);
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
		if(!!player)
			bRanged = true;
	}
}

void ABoss::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!!OtherActor && Alive())
	{
		bRanged = false;
	}
}

//Attack A & B
void ABoss::AttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (!!OtherActor)
	{

		ACPlayer* player = Cast<ACPlayer>(OtherActor);
		CLog::Print("player attack");
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
void ABoss::AttacCkBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void ABoss::AttacCkEndnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
