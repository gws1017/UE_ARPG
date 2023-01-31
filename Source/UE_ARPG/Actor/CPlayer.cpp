#include "Actor/CPlayer.h"
#include "Actor/Enemy.h"
#include "Actor/Weapon.h"
#include "Actor/LongSword.h"
#include "Actor/CAnimInstance.h"
#include "UI/HUDOverlay.h"
#include "Global.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"


ACPlayer::ACPlayer()
	: MaxHP(15), HP(15),
	MaxStamina(50), Stamina(50), StaminaRegenRate(2.f),
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

	UHelpers::GetClass(&HUDOverlayClass, "WidgetBlueprint'/Game/Character/UI/BP_HUDOverlay.BP_HUDOverlay_C'");
	PlayerHUDOverlay = Cast<UHUDOverlay>(CreateWidget(GetWorld(), HUDOverlayClass));

	UHelpers::GetAsset<UAnimMontage>(&DeathMontage, "AnimMontage'/Game/Character/Montage/Death_Montage.Death_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&HitMontage, "AnimMontage'/Game/Character/Montage/Hit2_Montage.Hit2_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&Attack1Montage, "AnimMontage'/Game/Character/Montage/Sword_Attack1_Montage.Sword_Attack1_Montage'");
	UHelpers::GetAsset<UAnimMontage>(&Attack2Montage, "AnimMontage'/Game/Character/Montage/Sword_Attack2_Montage.Sword_Attack2_Montage'");

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
	if(!!PlayerHUDOverlay)
		PlayerHUDOverlay->AddToViewport();
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//정확한 위치를 찾아내기가 쉽지않다
	if (bHitMovement && bHit){
		auto delta_loc = -GetActorForwardVector() * GetCharacterMovement()->MaxWalkSpeed * DeltaTime;
		SetActorLocation(GetActorLocation() + delta_loc);
	}
	
	float DeltaStamina = StaminaRegenRate * DeltaTime;
	UpdateStamina(DeltaStamina);
}

void ACPlayer::UpdateStamina(float DeltaStamina)
{
	CheckTrue(MovementStatus == EMovementStatus::EMS_Dead); //죽었을 때 종료
	CheckTrue((Stamina == MaxStamina) && (MovementStatus != EMovementStatus::EMS_Sprinting)); //스테미나 변동이 없을 시 종료

	if (MovementStatus == EMovementStatus::EMS_Sprinting && FMath::IsNearlyZero(GetVelocity().Length()) == false)
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

	PlayerInputComponent->BindAction("ReadyWeapon", EInputEvent::IE_Pressed, this, &ACPlayer::ReadyWeapon);

	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &ACPlayer::OnAttack);
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
		//피격 이펙트 및 사운드 추가부분
		//사운드는 무기에서 얻고 피격 이펙트는 맞는 대상에서 가져온다
		enemy->Hit();
		UGameplayStatics::ApplyDamage(OtherActor, Weapon->GetDamage(), GetController(), Weapon,TSubclassOf<UDamageType>());

	}
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
		CLog::Print("AttackMontage");
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
	if (bHitMovement == true ) bHitMovement = false;
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

bool ACPlayer::Alive()
{
	if (MovementStatus != EMovementStatus::EMS_Dead)
		return true;
	else return false;
}

void ACPlayer::Hit()
{
	CheckFalse(Alive());

	SetMovementStatus(EMovementStatus::EMS_Hit);
	PlayAnimMontage(HitMontage);
	bHit = true;
}

void ACPlayer::HitEnd()
{
	SetMovementStatus(EMovementStatus::EMS_Normal);
	bHit = false;
}

bool ACPlayer::CanAttack()
{
	CheckFalseResult(Weapon->GetEquipped(),false);
	CheckTrueResult(Weapon->GetEquipping(),false);
	switch (MovementStatus)
	{
	case EMovementStatus::EMS_Dead:
	case EMovementStatus::EMS_Hit:
		return false;
	default:
		if (GetWeapon()->GetStaminaCost() < Stamina)
			return true;
		else return false;

	}
}

void ACPlayer::ResetCombo()
{
		CLog::Print("Reset Comob");
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
