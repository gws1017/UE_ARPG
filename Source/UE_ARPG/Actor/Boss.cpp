#include "Actor/Boss.h"
#include "Actor/CPlayer.h"
#include "Actor/Stone.h"
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

	TSubclassOf<UAnimInstance> anim;
	UHelpers::GetClass(&anim, "AnimBlueprint'/Game/Enemy/BossBear/Animation/ABP_Bear.ABP_Bear_C'");
	GetMesh()->SetAnimInstanceClass(anim);

	UHelpers::GetAsset<UAnimMontage>(&AttackMontage, "AnimMontage'/Game/Enemy/BossBear/Montage/Bear_Attack_Montage.Bear_Attack_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&DeathMontage, "AnimMontage'/Game/Enemy/BossBear/Montage/Bear_Death_Montage.Bear_Death_Montage'");

	UHelpers::GetAsset<UParticleSystem>(&AttackCParticle, "ParticleSystem'/Game/Enemy/BossBear/FX/p_RipplingSmash_SegmentFX.p_RipplingSmash_SegmentFX'");

	UHelpers::SocketAttachComponent<UCapsuleComponent>(this, &LWeaponCollision, "LWeaponCollision", GetMesh(), "hand_lf");
	UHelpers::SocketAttachComponent<UCapsuleComponent>(this, &RWeaponCollision, "RWeaponCollision", GetMesh(), "hand_rt");
	UHelpers::CreateComponent<USphereComponent>(this, &AtkCSphere, "AtkCSphere",GetMesh());
	UHelpers::CreateComponent<USphereComponent>(this, &RangedAtkSphere, "RangedAtkSphere", GetRootComponent());

	WeaponInstigator = GetController();
	Damage = 5;
	DamageC = 7;

	SectionList.Add("AttackA");
	SectionList.Add("AttackB");
	SectionList.Add("AttackC");
	SectionList.Add("AttackThrow");

	MaxHP = 50;
	HP = MaxHP;

	GetCapsuleComponent()->SetCapsuleHalfHeight(150.f);
	GetCapsuleComponent()->SetCapsuleRadius(130.f); RWeaponCollision->SetCapsuleHalfHeight(60);

	RWeaponCollision->SetCapsuleRadius(40);
	LWeaponCollision->SetCapsuleHalfHeight(60);
	LWeaponCollision->SetCapsuleRadius(40);

	SetWeaponCollision(&RWeaponCollision, FVector(-8, -16, -1.5), FRotator(-80, 0, -30));
	SetWeaponCollision(&LWeaponCollision, FVector(8, 16, 1.5), FRotator(-80, 0, -30));
	SetWeaponCollision(&AtkCSphere, FVector(0, 155, -3), FRotator(0, 0, 0));
	AtkCSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	AtkCSphere->InitSphereRadius(80.f);
	RangedAtkSphere->InitSphereRadius(350.f);
	AgroSphere->InitSphereRadius(500.f);
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
	AtkCSphere->OnComponentBeginOverlap.AddDynamic(this, &ABoss::AttackCBeginOverlap);
	AtkCSphere->OnComponentEndOverlap.AddDynamic(this, &ABoss::AttackCEndnOverlap);
	RangedAtkSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	RangedAtkSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

}

void ABoss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//Empty�� �޸𸮱�������
	//Reset�� �����͸� ����� �޸�(����)�� ���ܵ�
	CollisionMap.Empty();
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

//�÷��̾ �����ȿ������� �ѹ��� ó��
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
		int32 num = 3;//FMath::FRandRange(0, 2);
		if (SectionList[num] == "AttackThrow") {
			CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		}
		else CombatSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		
		AnimInstance->Montage_JumpToSection(SectionList[num]);
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
			//��ǥ�� �߰��ϸ� ��ǥ���� �ٰ������ΰ�?
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

			//��Ÿ����̸� �̵�����?
		}
	}
}

void ABoss::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!!OtherActor && Alive())
	{
		CheckNull(CombatTarget); //��ǥ�� ������ ����
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
			//�ǰ� ����Ʈ �� ���� �߰��κ�
			//����� ���⿡�� ��� �ǰ� ����Ʈ�� �´� ��󿡼� �����´�
			player->Hit();
			UGameplayStatics::ApplyDamage(player, Damage, WeaponInstigator, this, DamageTypeClass);
			bDamaged = true;
		}
		
	}
}

//�÷��̾ �����ȿ��ִ��� �׻� Ȯ���Ѵ�.
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
