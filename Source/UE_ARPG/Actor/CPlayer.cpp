#include "Actor/CPlayer.h"
#include "Actor/Enemy.h"
#include "Actor/Weapon.h"
#include "Actor/LongSword.h"
#include "Actor/CAnimInstance.h"
#include "Actor/CPlayerController.h"
#include "Actor/LostExp.h"
#include "Utilities/MySaveGame.h"
#include "Global.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"

ACPlayer::ACPlayer()
	: Stat{15,15,50,50,0,1,1,1,1,0}, StartPoint{150, 0, 190},
	StaminaRegenRate(2.f),RollStamina(10.f),
	MovementState(EMovementState::EMS_Normal), PlayerStat(EPlayerStat::EPS_Normal)
{
	PrimaryActorTick.bCanEverTick = true;

	UHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	UHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
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


	UHelpers::CreateComponent<UAudioComponent>(this, &AudioComponent,"AttackSound", GetRootComponent());

	AudioComponent->SetAutoActivate(false);

	SpringArm->SetRelativeLocation(FVector(0, 0, 30));
	SpringArm->TargetArmLength = 200.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0, 60, 0);
	Tags.Add("Player");
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	Weapon = AWeapon::Spawn<ALongSword>(GetWorld(), this);
	PlayerController = GetController<ACPlayerController>();
	LoadGameData();
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
	CheckTrue((Stat.Stamina == Stat.MaxStamina) && (MovementState != EMovementState::EMS_Sprinting)); //스테미나 변동이 없을 시 종료

	if (MovementState == EMovementState::EMS_Sprinting && FMath::IsNearlyZero(GetVelocity().Length()) == false)
	{
		Stat.Stamina -= DeltaStamina;
		Stat.Stamina = FMath::Clamp(Stat.Stamina, 0.f, Stat.MaxStamina);
		if (Stat.Stamina <= 0)
		{
			OffRunning();
		}
		return;
	}
	
	Stat.Stamina += DeltaStamina;

	Stat.Stamina = FMath::Clamp(Stat.Stamina, 0.f, Stat.MaxStamina);
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

void ACPlayer::IncreamentExp(int32 e)
{
	Stat.Exp += e;
}
void ACPlayer::DecrementStamina(float Amount)
{
	Stat.Stamina = FMath::Clamp(Stat.Stamina-Amount, 0.f, Stat.MaxStamina);
}

bool ACPlayer::CanMove()
{
	CheckFalseResult(Alive(), false);
	if (PlayerController) 
	{
		CheckFalseResult(PlayerController->GetGameMode(),false);
	}
	return true;
}

void ACPlayer::OnMoveForward(float Axis)
{
	if (CanMove()) {
		FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
		FVector dir = FQuat(rotator).GetForwardVector().GetUnsafeNormal2D();

		AddMovementInput(dir, Axis);
	}
}


void ACPlayer::OnMoveRight(float Axis)
{
	if (CanMove()) {
		FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
		FVector dir = FQuat(rotator).GetRightVector().GetUnsafeNormal2D();

		AddMovementInput(dir, Axis);
	}
}

void ACPlayer::OnHorizonLock(float Axis)
{
	if(CanMove())AddControllerYawInput(Axis);
}

void ACPlayer::OnVerticalLock(float Axis)
{
	if (CanMove())AddControllerPitchInput(Axis);
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
	if (CanMove()) {
		SetMovementState(EMovementState::EMS_Sprinting);
		GetCharacterMovement()->MaxWalkSpeed = 400;
	}
}

void ACPlayer::OffRunning()
{
	if (CanMove()) {
		SetMovementState(EMovementState::EMS_Normal);
		GetCharacterMovement()->MaxWalkSpeed = 300;
	}
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
	SaveGameData(1);

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

	if (AudioComponent->Sound)
		AudioComponent->Play();

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
	if(PlayerController)CheckFalseResult(PlayerController->GetGameMode(),false);
	CheckFalseResult(Weapon->GetEquipped(),false);
	CheckTrueResult(Weapon->GetEquipping(),false);
	switch (MovementState)
	{
	case EMovementState::EMS_Dead:
	case EMovementState::EMS_Hit:
	case EMovementState::EMS_Roll:
		return false;
	default:
		if (GetWeapon()->GetStaminaCost() < Stat.Stamina)
			return true;
		else return false;

	}
}

bool ACPlayer::CanRoll()
{
	CheckTrueResult(bAttacking,false);
	if (PlayerController)CheckFalseResult(PlayerController->GetGameMode(),false);

	switch (MovementState)
	{
	case EMovementState::EMS_Dead:
	case EMovementState::EMS_Hit:
	case EMovementState::EMS_Roll:
		return false;
	default:
		if (Stat.Stamina - RollStamina > 0)
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
void ACPlayer::LevelUp(const FPlayerStatus& data)
{
	Stat = data;
	SaveGameData();
}

float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount <= 0.f || EPlayerStat::EPS_Invincible == PlayerStat)
		return DamageAmount;

	if (Stat.HP - DamageAmount <= 0.f)
	{
		Stat.HP = FMath::Clamp(Stat.HP - DamageAmount, 0.0f, Stat.MaxHP);
		AEnemy* enemy = Cast<AEnemy>(DamageCauser);
		if (enemy)
		{
			enemy->InitTarget();
			enemy->SetAlerted(false);
			enemy->ClearAttackTimer();
		}
		Die();
	}
	else
	{
		Stat.HP = FMath::Clamp(Stat.HP - DamageAmount, 0.0f, Stat.MaxHP);
	}
	
	UE_LOG(LogTemp, Display, L"Player Current HP : %f", Stat.HP);
	return DamageAmount;
}

void ACPlayer::SaveGameData(int32 SaveType)
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	
	SaveGameInstance->SaveData = {
		Stat,
		0,
		GetActorLocation(),
		GetActorRotation(),
		StartPoint,
		DeathLocation
	};

	if (SaveType == 1)//죽었을때 수정해야하는 부분
	{
		SaveGameInstance->SaveData.Location = StartPoint;
		SaveGameInstance->SaveData.LostExp = Stat.Exp; //현재경험치를 LostExp로 저장, 평소에는 0이 기본
		SaveGameInstance->SaveData.Status.Exp = 0;
		SaveGameInstance->SaveData.Status.HP = Stat.MaxHP;
		SaveGameInstance->SaveData.DeathLocation = GetActorLocation();
	}
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);
}

void ACPlayer::LoadGameData()
{
	UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));

	if (LoadGameInstance)
	{
		FSaveData Data = LoadGameInstance->SaveData;
		Stat = Data.Status;
		Stat.Stamina = Stat.MaxStamina;
		SetActorLocation(Data.Location);
		SetActorRotation(Data.Rotation);
		StartPoint = Data.StartPoint;
		if (Data.LostExp != 0) {
			if (LostExpClass)
			{
				ALostExp* actor = GetWorld()->SpawnActor<ALostExp>(LostExpClass);
				FVector Loc = Data.DeathLocation;
				Loc.Z -= 90.f;
				actor->Init(Data.LostExp, Loc);
			}
			else CLog::Log("LostExp BP is not valid");
		}

		SetMovementState(EMovementState::EMS_Normal);
		GetMesh()->bPauseAnims = false;
		GetMesh()->bNoSkeletonUpdate = false;
	}
	else CLog::Log("SaveData is not valid");
}

float ACPlayer::GetDamage()
{
	float Damage = Stat.StrengthDamage;
	if (Weapon)
	{
		Damage += Weapon->GetDamage();
	}
	return Damage;
}
