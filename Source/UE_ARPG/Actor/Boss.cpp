#include "Actor/Boss.h"
#include "Actor/CPlayer.h"
#include "Global.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

ABoss::ABoss()
{
	USkeletalMesh* mesh;
	UHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Enemy/BossBear/Mesh/Boss_Bear.Boss_Bear'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -140));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	//GetMesh()->SetRelativeScale3D(FVector(0.8, 0.8, 0.8));

	GetCapsuleComponent()->SetCapsuleHalfHeight(150.f);
	GetCapsuleComponent()->SetCapsuleRadius(130.f);

	TSubclassOf<UAnimInstance> anim;
	UHelpers::GetClass(&anim, "AnimBlueprint'/Game/Enemy/BossBear/Animation/ABP_Bear.ABP_Bear_C'");
	GetMesh()->SetAnimInstanceClass(anim);


	auto SetWeaponCollision = [this](UCapsuleComponent** Collision,float hh, float r, FVector Location, FRotator Rotation)
	{
		(*Collision)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		(*Collision)->SetCapsuleHalfHeight(hh);
		(*Collision)->SetCapsuleRadius(r);
		(*Collision)->SetRelativeLocation(Location);
		(*Collision)->SetRelativeRotation(Rotation);
		
		CollisionMap.Add((*Collision)->GetName(), *Collision);
		(*Collision)->bHiddenInGame = false; //Debug

	};

	UHelpers::SocketAttachComponent<UCapsuleComponent>(this, &LWeaponCollision, "LWeaponCollision", GetMesh(), "hand_lf");
	SetWeaponCollision(&LWeaponCollision,60,40, FVector(8, 16, 1.5), FRotator(-80, 0, -30));
	UHelpers::SocketAttachComponent<UCapsuleComponent>(this, &RWeaponCollision, "RWeaponCollision", GetMesh(), "hand_rt");
	SetWeaponCollision(&RWeaponCollision,60,40, FVector(-8, -16, -1.5), FRotator(-80, 0, -30));

	WeaponInstigator = GetController();
	Damage = 5;

	UHelpers::GetAsset<UAnimMontage>(&AttackMontage, "AnimMontage'/Game/Enemy/BossBear/Montage/Bear_Attack_Montage.Bear_Attack_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&DeathMontage, "AnimMontage'/Game/Enemy/BossBear/Montage/Bear_Death_Montage.Bear_Death_Montage'");

	SectionList.Add("AttackA");
	SectionList.Add("AttackB");

	MaxHP = 50;
	HP = MaxHP;

	AgroSphere->InitSphereRadius(300.f);
	CombatSphere->InitSphereRadius(170.f);
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	auto SetWeaponCollision = [this](UCapsuleComponent** Collision)
	{
		(*Collision)->OnComponentBeginOverlap.AddDynamic(this, &ABoss::AttackBeginOverlap);
		(*Collision)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		(*Collision)->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		(*Collision)->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		(*Collision)->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	};
	SetWeaponCollision(&LWeaponCollision);
	SetWeaponCollision(&RWeaponCollision);
	/*LWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoss::AttackBeginOverlap);
	LWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LWeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LWeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LWeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);*/
	//RWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoss::AttackBeginOverlap);
}

void ABoss::Begin_Collision(FString name)
{
	CollisionMap[name]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABoss::End_Collision(FString name)
{
	CollisionMap[name]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bDamaged = false;

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
	AEnemy::Attack();

	bAttacking = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!!AnimInstance)
	{
		AnimInstance->Montage_Play(AttackMontage);
		int32 num = FMath::FRandRange(0, 2);
		AnimInstance->Montage_JumpToSection(SectionList[num]);
	}
}

void ABoss::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!!OtherActor && Alive())
	{
		ACPlayer* player = Cast<ACPlayer>(OtherActor);
		if (player)
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
		if (player)
		{
			CombatTarget = nullptr;
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
		bRanged = true;
	}
}

void ABoss::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!!OtherActor && Alive())
	{
		CheckNull(CombatTarget);
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
