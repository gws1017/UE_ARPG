#include "UI/LevelUpUI.h"
#include "MyGameInstance.h"
#include "DataTable/CharacterAbilityTables.h"
#include "UI/OKUI.h"
#include "Global.h"

#include "Components/Border.h"
#include "Components/Button.h"

void ULevelUpUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	PlayerInstance = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	GameInstance = GetGameInstance<UMyGameInstance>();

	AbilityArray = { VigBorder,EndureBorder,StrBorder };

	OKBtnDisable();
}


void ULevelUpUI::NativeConstruct()
{
	Super::NativeConstruct();

	SelectUINumber = 0;
	TotalExp = 0;
	NextExp = GameInstance->GetCharAbilityData(GetChangePlayerLevel() + 1)->LevelUpExp;
	LevelUpCount.Init(0, AbilityArray.Num());
	SelectAbility();
}

void ULevelUpUI::NativeDestruct()
{
	Super::NativeDestruct();
}

void ULevelUpUI::OKBtnEnable()
{
	OKBtn->SetIsEnabled(true);
	OKBtn->SetColorAndOpacity(FLinearColor::White);
}

void ULevelUpUI::OKBtnDisable()
{
	OKBtn->SetIsEnabled(false);
	OKBtn->SetColorAndOpacity(FLinearColor(0.05f, 0.05f, 0.05f));
}


void ULevelUpUI::LevelUp()
{
	FPlayerStatus ChangeData = {
		static_cast<float>(GetChangeMaxHP()),
		static_cast<float>(GetChangeMaxHP()),
		static_cast<float>(GetChangeMaxStamina()),
		static_cast<float>(GetChangeMaxStamina()),
		GameInstance->GetCharAbilityData(GetChangeStrength())->TotalDmgIncrease,
		GetChangeVigor(),
		GetChangeEnduarance(),
		GetChangeStrength(),
		GetChangePlayerLevel(),
		GetChangePlayerExp()
	};
	
	PlayerInstance->LevelUp(ChangeData);
	TotalExp = 0;
	NextExp = GameInstance->GetCharAbilityData(PlayerInstance->GetPlayerLevel() + 1)->LevelUpExp;
	LevelUpCount.Init(0, AbilityArray.Num());
	OKBtnDisable();
	bUpdatedAbility = false;
}

void ULevelUpUI::SelectAbility()
{
	for (auto ability : AbilityArray) {
		ability->SetBrushColor( FLinearColor(0.f,0.f,0.f,0.f) );
	}
	AbilityArray[SelectUINumber]->SetBrushColor(FLinearColor(0.7f,0.7f,0.7f,1.0f));
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
			UpdateExp();
		}
	}
}

void ULevelUpUI::OnRightKey()
{
	if (IsInViewport()) {
		//요구 경험치이상을 보유해야 레벨업 가능
		LevelUpCount[SelectUINumber]++;
		if (GetPlayerExp() < TotalExp + NextExp)
			LevelUpCount[SelectUINumber]--;
		else
			UpdateExp();
	}
}

void ULevelUpUI::OnClickOKBtn()
{
	OKUI = Cast<UOKUI>(CreateWidget(GetWorld(), OKUIIClass));

	if (OKUI) 
	{
		OKUI->ClickOKBtnFunction.BindUFunction(this, "LevelUp");
		OKUI->AddToViewport();
	}
}

int32 ULevelUpUI::GetChangePlayerExp()
{
	if (bUpdatedAbility == false) {
		return PlayerInstance->GetExp(); 
	}
	else return PlayerInstance->GetExp() - TotalExp;

}

int32 ULevelUpUI::GetPlayerNextExp()
{
	if (GameInstance && bUpdatedAbility)
	{
		return  static_cast<int32>
			(GameInstance->GetCharAbilityData(GetChangePlayerLevel()+1)->LevelUpExp);
	}
	return NextExp;
}

void ULevelUpUI::UpdateExp()
{
	int32 CurrentLevel = GetPlayerLevel();
	int32 TargetLevel = GetChangePlayerLevel();
	int32 delta_level = TargetLevel - CurrentLevel;

	if (delta_level > 0)
	{
		bUpdatedAbility = true;
		OKBtnEnable();
		int32 SumExp = 0;
		for (int i = CurrentLevel; i < TargetLevel; ++i) 
		{
			SumExp += GameInstance->GetCharAbilityData(i+1)->LevelUpExp;
		}
		TotalExp = SumExp;
	}
	else {
		OKBtnDisable();
		bUpdatedAbility = false;
		TotalExp = 0;
	}
	NextExp = static_cast<int32>
		(GameInstance->GetCharAbilityData(GetChangePlayerLevel() + 1)->LevelUpExp);
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
	if (GameInstance && bUpdatedAbility)
	{
		return  static_cast<int32>
			(GameInstance->GetCharAbilityData(GetChangeVigor())->TotalHP);
	}
	return GetMaxHP();
}

int32 ULevelUpUI::GetChangeMaxStamina()
{
	if (GameInstance && bUpdatedAbility)
	{
		return  static_cast<int32>
			(GameInstance->GetCharAbilityData(GetChangeEnduarance())->TotalStamina );
	}
	return GetMaxStamina();
}

int32 ULevelUpUI::GetChangePlayerDamage()
{
	if (GameInstance && bUpdatedAbility)
	{
		return  static_cast<int32>
			(GameInstance->GetCharAbilityData(GetChangeStrength())->TotalDmgIncrease +
				PlayerInstance->GetDamage());
	}
	return GetPlayerDamage();
}

