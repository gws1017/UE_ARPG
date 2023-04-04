#include "Actor/CPlayer.h"
#include "Actor/Enemy.h"
#include "Actor/Weapon.h"
#include "Actor/LongSword.h"
#include "Actor/CAnimInstance.h"
#include "Actor/CPlayerController.h"
#include "Global.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundCue.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"

ACPlayer::ACPlayer()
	: MaxHP(15), HP(15),
	MaxStamina(50), Stamina(50), StaminaRegenRate(2.f),
	RollStamina(10.f),
	MovementState(EMovementState::EMS_Normal), PlayerStat(EPlayerState::EPS_Normal)
{
	PrimaryActorTick.bCanEverTick = true;

	UHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	UHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->SetWalkableFloorAngle(70.f);

	USkeletalMesh* mesh;
	UHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/Eve_By_J_Gonzales.Eve_By_J_Gonzales'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	TSubclassOf<UAnimInstance> anim;
	UHelpers::GetClass(&anim, "AnimBlueprint'/Game/Character/Animation/ABP_Player.ABP_Player_C'");
	GetMesh()->SetAnimInstanceClass(anim);

	UHelpers::GetAsset<UAnimMontage>(&DeathMontage, "AnimMontage'/Game/Character/Montage/Death_Montage.Death_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&HitMontage, "AnimMontage'/Game/Character/Montage/Hit2_Montage.Hit2_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&Attack1Montage, "AnimMontage'/Game/Character/Montage/Sword_Attack1_Montage.Sword_Attack1_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&Attack2Montage, "AnimMontage'/Game/Character/Montage/Sword_Attack2_Montage.Sword_Attack2_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&RollMontage, "AnimMontage'/Game/Character/Montage/Roll_Montage.Roll_Montage'");


	UHelpers::CreateComponent<UAudioComponent>(this, &AttackAudioComponent,"AttackSound", GetRootComponent());
	UHelpers::GetAsset<USoundCue>(&AttackSoundCue, "SoundCue'/Game/Audio/SwordAttackCue.SwordAttackCue'");

	AttackAudioComponent->SetSound(AttackSoundCue);
	AttackAudioComponent->SetAutoActivate(false);

	SpringArm->SetRelativeLocation(FVector(0, 0, 30));
	SpringArm->TargetArmLength = 200.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0, 60, 0);

}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	Weapon = AWeapon::Spawn<ALongSword>(GetWorld(), this);
	Weapon->GetWeaponCollision()->OnComponentBeginOverlap.AddDynamic(this, &ACPlayer::WeaponBeginOverlap);

}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float DeltaStamina = StaminaRegenRate * DeltaTime;
	UpdateStamina(DeltaStamina);
}

void ACPlayer::UpdateStamina(float DeltaStamina)
{
	CheckTrue(MovementState == EMovementState::EMS_Dead); //죽었을 때 종료
	CheckTrue((Stamina == MaxStamina) && (MovementState != EMovementState::EMS_Sprinting)); //스테미나 변동이 없을 시 종료

	if (MovementState == EMovementState::EMS_Sprinting && FMath::IsNearlyZero(GetVelocity().Length()) == false)
	{
		Stamina -= DeltaStamina;
		Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
		if (Stamina <= 0)
		{
			OffRunning();
		}
		return;
	}
	
	Stamina += DeltaStamina;

	Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
	return;
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizonLock", this, &ACPlayer::OnHorizonLock);
	PlayerInputComponent->BindAxis("VerticalLock", this, &ACPlayer::OnVerticalLock);

	PlayerInputComponent->BindAction("Running", EInputEvent::IE_Pressed, this, &ACPlayer::OnRunning);
	PlayerInputComponent->BindAction("Running", EInputEvent::IE_Released, this, &ACPlayer::OffRunning);

	PlayerInputComponent->BindAction("ESC", EInputEvent::IE_Pressed, this, &ACPlayer::ESCDown);
	PlayerInputComponent->BindAction("ESC", EInputEvent::IE_Released, this, &ACPlayer::ESCUp);

	PlayerInputComponent->BindAction("ReadyWeapon", EInputEvent::IE_Pressed, this, &ACPlayer::ReadyWeapon);

	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &ACPlayer::OnAttack);

	PlayerInputComponent->BindAction("Roll", EInputEvent::IE_Pressed, this, &ACPlayer::OnRoll);
}

void ACPlayer::DecrementStamina(float Amount)
{
	Stamina = FMath::Clamp(Stamina-Amount, 0.f, MaxStamina);
}

void ACPlayer::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* enemy = Cast<AEnemy>(OtherActor);
	if (!!enemy)
	{
		enemy->Hit(Weapon->GetActorLocation());
		UGameplayStatics::ApplyDamage(OtherActor, Weapon->GetDamage(), GetController(), Weapon,TSubclassOf<UDamageType>());
	}
}

void ACPlayer::OnMoveForward(float Axis)
{
	CheckFalse(Alive());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector dir = FQuat(rotator).GetForwardVector().GetUnsafeNormal2D();

	AddMovementInput(dir, Axis);

}


void ACPlayer::OnMoveRight(float Axis)
{
	CheckFalse(Alive());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector dir = FQuat(rotator).GetRightVector().GetUnsafeNormal2D();

	AddMovementInput(dir, Axis);

}

void ACPlayer::OnHorizonLock(float Axis)
{
	AddControllerYawInput(Axis);
}

void ACPlayer::OnVerticalLock(float Axis)
{
	AddControllerPitchInput(Axis);
}

void ACPlayer::OnRoll()
{
	//구르는 도중 달리기 공격을 어떻게 막을것인가?
	if (CanRoll()) {
		PlayAnimMontage(RollMontage);
		DecrementStamina(RollStamina);
		
	}
}

void ACPlayer::OnRunning()
{
	CheckFalse(Alive());
	SetMovementState(EMovementState::EMS_Sprinting);
	GetCharacterMovement()->MaxWalkSpeed = 400;
}

void ACPlayer::OffRunning()
{
	CheckFalse(Alive());
	SetMovementState(EMovementState::EMS_Normal);
	GetCharacterMovement()->MaxWalkSpeed = 200;
}

void ACPlayer::ESCDown()
{
	GetController<ACPlayerController>()->TogglePauseMenu();
}

void ACPlayer::ESCUp()
{
}


void ACPlayer::ReadyWeapon()
{
	CheckFalse(Alive());
	CheckNull(Weapon);
	if (Weapon->GetEquipped())
	{
		Weapon->UnEquip();
		return;
	}

	Weapon->Equip();
}

void ACPlayer::OnAttack()
{
	if (CanAttack())
	{
		if (bAttacking == true)
		{
			bSaveAttack = true;
		}
		else
		{
			bAttacking = true;
			PlayAttackMontage();
		}
	}
}

void ACPlayer::PlayAttackMontage()
{
		switch (AttackCount)
		{
		case 0:
			AttackCount = 1;
			PlayAnimMontage(Attack1Montage);
			break;
		case 1:
			AttackCount = 0;
			PlayAnimMontage(Attack2Montage);
			break;
		}
		DecrementStamina(Weapon->GetStaminaCost());
}

void ACPlayer::Begin_Attack()
{
}

void ACPlayer::End_Attack()
{
	//Notify로 호출
}



void ACPlayer::Die()
{
	SetMovementState(EMovementState::EMS_Dead);
	GetCharacterMovement()->StopMovementImmediately();

	StopAnimMontage();

	PlayAnimMontage(DeathMontage);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (!!Weapon)
		Weapon->DeactivateCollision();

	GetController<ACPlayerController>()->ShowRestartenu();

}

void ACPlayer::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

}

bool ACPlayer::Alive()
{
	if (MovementState != EMovementState::EMS_Dead)
		return true;
	else return false;
}

void ACPlayer::Hit(const FVector& ParticleSpawnLocation)
{
	CheckFalse(Alive());

	ResetCombo();
	SetMovementState(EMovementState::EMS_Hit);
	PlayAnimMontage(HitMontage);
	bHit = true;
}


void ACPlayer::HitEnd()
{
	SetMovementState(EMovementState::EMS_Normal);
	bHit = false;
}

bool ACPlayer::CanAttack()
{
	CheckFalseResult(Weapon->GetEquipped(),false);
	CheckTrueResult(Weapon->GetEquipping(),false);
	switch (MovementState)
	{
	case EMovementState::EMS_Dead:
	case EMovementState::EMS_Hit:
	case EMovementState::EMS_Roll:
		return false;
	default:
		if (GetWeapon()->GetStaminaCost() < Stamina)
			return true;
		else return false;

	}
}

bool ACPlayer::CanRoll()
{
	CheckTrueResult(bAttacking,false);

	switch (MovementState)
	{
	case EMovementState::EMS_Dead:
	case EMovementState::EMS_Hit:
	case EMovementState::EMS_Roll:
		return false;
	default:
		if (Stamina - RollStamina > 0)
			return true;
		else return false;

	}
}

void ACPlayer::ResetCombo()
{
		CLog::Log("Reset Comob");
		AttackCount = 0;
		bAttacking = false;
		bSaveAttack = false;
}

void ACPlayer::ComboAttackSave()
{
	if (bSaveAttack)
	{
		bSaveAttack = false;
		PlayAttackMontage();
	}
}

float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount <= 0.f || EPlayerState::EPS_Invincible == PlayerStat)
		return DamageAmount;

	if (HP - DamageAmount <= 0.f) 
	{
		HP = FMath::Clamp(HP - DamageAmount, 0.0f, MaxHP);
		Die();
	}
	else
	{
		HP = FMath::Clamp(HP - DamageAmount, 0.0f, MaxHP);
		
	}

	
	UE_LOG(LogTemp, Display, L"Player Current HP : %f", HP);
	return DamageAmount;
}
