#include "UI/LevelUpUI.h"
#include "Actor/CPlayer.h"
#include "MyGameInstance.h"
#include "DataTable/CharacterAbilityTables.h"
#include "Global.h"

#include "Components/Border.h"

void ULevelUpUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	PlayerInstance = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	GameInstance = GetGameInstance<UMyGameInstance>();

	AbilityArray = { VigBorder,EndureBorder,StrBorder };
}

void ULevelUpUI::NativeConstruct()
{
	Super::NativeConstruct();

	SelectUINumber = 0;
	TargetExp = PlayerInstance->GetLevelUpExp();
	LevelUpCount.Init(0, AbilityArray.Num());
	SelectAbility();
}

void ULevelUpUI::NativeDestruct()
{
	Super::NativeDestruct();
}

void ULevelUpUI::SelectAbility()
{
	for (auto ability : AbilityArray) {
		ability->SetBrushColor(FLinearColor::Black);
	}
	AbilityArray[SelectUINumber]->SetBrushColor(FLinearColor::White);
}

void ULevelUpUI::OnUpKey()
{
	if (IsInViewport()) {
		SelectUINumber--;
		if (SelectUINumber < 0) SelectUINumber = AbilityArray.Num() - 1;
		SelectAbility();
	}
	
}

void ULevelUpUI::OnDownKey()
{
	if (IsInViewport()) {
		SelectUINumber++;
		if (SelectUINumber >= AbilityArray.Num()) SelectUINumber = 0;
		SelectAbility();
	}
}

void ULevelUpUI::OnLeftKey()
{
	if (IsInViewport()) {
		if (LevelUpCount[SelectUINumber] > 0) {
			LevelUpCount[SelectUINumber]--;
			UpdateTargetExp();
		}
	}
}

void ULevelUpUI::OnRightKey()
{
	if (IsInViewport()) {
		//요구 경험치이상을 보유해야 레벨업 가능
			LevelUpCount[SelectUINumber]++;
			UpdateTargetExp();
			if (GetPlayerExp() < TargetExp)
			{
				LevelUpCount[SelectUINumber]--;
				UpdateTargetExp();
			}
	}
}

int32 ULevelUpUI::GetChangePlayerExp()
{
	if (GetChangePlayerLevel() - GetPlayerLevel() == 0) {
		return PlayerInstance->GetExp(); 
	}
	else return PlayerInstance->GetExp() - TargetExp;

}

void ULevelUpUI::UpdateTargetExp()
{
	int32 CurrentLevel = GetPlayerLevel();
	int32 TargetLevel = GetChangePlayerLevel();
	int32 dl = TargetLevel - CurrentLevel;

	int32 LevelUpExp = PlayerInstance->GetLevelUpExp();
	int32 SumExp = LevelUpExp;
	if (TargetLevel > 0) {
		for (int i = 0; i < dl; ++i) {
			if (TargetLevel <= 12 && TargetLevel >= 2) {
				SumExp += SumExp * FMath::Pow((1.025), dl);
			}
			else if (TargetLevel >= 13) {
				SumExp += 0.02 * FMath::Pow(static_cast<float>(TargetLevel), 3.0f) + 3.06 * FMath::Pow(static_cast<float>(TargetLevel), 2.0f)
					+ 105.6 * (TargetLevel)-895;
			}
		}
		TargetExp = SumExp;
	}
}

int32 ULevelUpUI::GetChangePlayerLevel()
{
	int32 sum = 0;
	for (const auto& cnt : LevelUpCount)
		sum += cnt;

	return sum + GetPlayerLevel();
}

int32 ULevelUpUI::GetChangeMaxHP()
{
	if (GameInstance)
	{
		return  static_cast<int32>
			(GameInstance->GetCharAbilityData(GetChangeVigor())->TotalHPIncrease + 
			PlayerInstance->GetMaxHP());
	}
	CLog::Log("In LevelUpUI, GameInstance Pointer is not intialized!");
	return GetMaxHP();
}

int32 ULevelUpUI::GetChangeMaxStamina()
{
	if (GameInstance)
	{
		return  static_cast<int32>
			(GameInstance->GetCharAbilityData(GetChangeEnduarance())->TotalStaIncrease +
				PlayerInstance->GetMaxStamina());
	}
	CLog::Log("In LevelUpUI, GameInstance Pointer is not intialized!");
	return GetMaxStamina();
}

int32 ULevelUpUI::GetChangePlayerDamage()
{
	if (GameInstance)
	{
		return  static_cast<int32>
			(GameInstance->GetCharAbilityData(GetChangeStrength())->TotalDmgIncrease +
				PlayerInstance->GetDamage());
	}
	CLog::Log("In LevelUpUI, GameInstance Pointer is not intialized!");
	return GetPlayerDamage();
}

