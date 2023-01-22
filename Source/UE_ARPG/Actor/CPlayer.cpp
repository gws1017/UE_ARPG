#include "Actor/CPlayer.h"
#include "Actor/Weapon.h"
#include "Actor/LongSword.h"
#include "Actor/CAnimInstance.h"
#include "Global.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"


ACPlayer::ACPlayer()
	: MaxHP(15), HP(15),
	MaxStamina(50), Stamina(50), StaminaRegenRate(20.f),
	MovementStatus(EMovementStatus::EMS_Normal)
{
	PrimaryActorTick.bCanEverTick = true;

	UHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	UHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	USkeletalMesh* mesh;
	UHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/Eve_By_J_Gonzales.Eve_By_J_Gonzales'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	TSubclassOf<UAnimInstance> anim;
	UHelpers::GetClass(&anim, "AnimBlueprint'/Game/Character/Animation/ABP_Player.ABP_Player_C'");
	GetMesh()->SetAnimInstanceClass(anim);

	UHelpers::GetAsset<UAnimMontage>(&DeathMontage, "AnimMontage'/Game/Character/Montage/Death_Montage.Death_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&HitMontage, "AnimMontage'/Game/Character/Montage/Hit_Montage.Hit_Montage'");

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
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (HP <= 0);
	
	CLog::Print(GetMesh()->GetBoneLocation("Hips"));
	float DeltaStamina = StaminaRegenRate * DeltaTime;
	UpdateStamina(DeltaStamina);
}

void ACPlayer::UpdateStamina(float DeltaStamina)
{
	CheckTrue(MovementStatus == EMovementStatus::EMS_Dead); //죽었을 때 종료
	CheckTrue((Stamina == MaxStamina) && (MovementStatus != EMovementStatus::EMS_Sprinting)); //스테미나 변동이 없을 시 종료

	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		Stamina -= DeltaStamina;
		Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
		if (Stamina <= 0)
		{
			OffRunning();
		}
		CLog::Print(Stamina, 1, 2.f);
		return;
	}
	
	Stamina += DeltaStamina;

	Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
	CLog::Print(Stamina,1,2.f);
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

	PlayerInputComponent->BindAction("ReadyWeapon", EInputEvent::IE_Pressed, this, &ACPlayer::ReadyWeapon);

	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &ACPlayer::OnAttack);
}

void ACPlayer::DecrementStamina(float Amount)
{
	CLog::Print("DecrementStamina!");
	//CLog::Print(Stamina, 2, 2.f);
	//CLog::Print(Amount, 3, 2.f);
	Stamina = FMath::Clamp(Stamina-Amount, 0.f, MaxStamina);

	
	return;
}

void ACPlayer::OnMoveForward(float Axis)
{
	if (Alive())
	{
		FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
		FVector dir = FQuat(rotator).GetForwardVector().GetUnsafeNormal2D();

		AddMovementInput(dir, Axis);
	}
}
	

void ACPlayer::OnMoveRight(float Axis)
{
	if (Alive())
	{
		FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
		FVector dir = FQuat(rotator).GetRightVector().GetUnsafeNormal2D();

		AddMovementInput(dir, Axis);
	}
}

void ACPlayer::OnHorizonLock(float Axis)
{
	AddControllerYawInput(Axis);
}

void ACPlayer::OnVerticalLock(float Axis)
{
	AddControllerPitchInput(Axis);
}

void ACPlayer::OnRunning()
{
	if (Alive()) {
		SetMovementStatus(EMovementStatus::EMS_Sprinting);
		GetCharacterMovement()->MaxWalkSpeed = 400;
	}
}

void ACPlayer::OffRunning()
{
	SetMovementStatus(EMovementStatus::EMS_Normal);
	GetCharacterMovement()->MaxWalkSpeed = 200;
}

void ACPlayer::ReadyWeapon()
{
	if (!!Weapon && Alive())
	{
		if (Weapon->GetEquipped())
		{
			Weapon->UnEquip();
			return;
		}

		Weapon->Equip();
	}
}

void ACPlayer::OnAttack()
{
	if (Alive())
	{
		Weapon->Attack();
	}
}

void ACPlayer::Die()
{
	SetMovementStatus(EMovementStatus::EMS_Dead);
	PlayAnimMontage(DeathMontage);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (!!Weapon)
		Weapon->DeactivateCollision();

}

void ACPlayer::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void ACPlayer::HitEnd()
{
	auto loc = -GetActorForwardVector() * GetCharacterMovement()->MaxWalkSpeed;
	//loc.Z = GetActorLocation().Z;
	
	SetActorLocation(loc);
}

bool ACPlayer::Alive()
{
	if (MovementStatus != EMovementStatus::EMS_Dead)
		return true;
	else return false;
}

void ACPlayer::Hit()
{
	PlayAnimMontage(HitMontage);
	//CLog::Print("Player play HitMontage");
}

float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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
	UE_LOG(LogTemp, Display, L"Player Current HP : %f", HP);
	return DamageAmount;
}
