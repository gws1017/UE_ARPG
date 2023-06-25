#include "UI/LevelUpUI.h"
#include "Actor/CPlayer.h"
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
	CLog::Print("LevelUP!");
	//����ü�� �����ؼ� �����°� ����������?
	//�÷��̾� Ŭ������ ������ �Լ��� �����, ������ �Լ���
	//����ȴɷ�ġ�� ����� ����ü�� �����Ѵ�
	//ui�� ������ ���ӻ����� ����������ϴ� �Ű�ü�� �̷��� �� �ڵ带
	//���� ���� ������������ �� ����
	
	if (PlayerInstance->GetMaxHP() != GetChangeMaxHP()) {
		PlayerInstance->SetMaxHP(GetChangeMaxHP());
		PlayerInstance->SetHP(GetMaxHP());
	}
	if (PlayerInstance->GetMaxStamina() != GetChangeMaxStamina())PlayerInstance->SetMaxStamina(GetChangeMaxStamina());
	if (PlayerInstance->GetDamage() != GetChangePlayerDamage())PlayerInstance->SetStrDamage(GameInstance->GetCharAbilityData(GetChangeStrength())->TotalDmgIncrease);
	
	if (PlayerInstance->GetExp() != GetChangePlayerExp())PlayerInstance->SetExp(GetChangePlayerExp());
	if (PlayerInstance->GetPlayerLevel() != GetChangePlayerLevel())PlayerInstance->SetPlayerLevel(GetChangePlayerLevel());
	if (PlayerInstance->GetVigor() != GetChangeVigor())PlayerInstance->SetVigor(GetChangeVigor());
	if (PlayerInstance->GetEnduarance() != GetChangeEnduarance())PlayerInstance->SetEnduarance(GetChangeEnduarance());
	if (PlayerInstance->GetStrength() != GetChangeStrength())PlayerInstance->SetStrength(GetChangeStrength());

	NextExp = GameInstance->GetCharAbilityData(PlayerInstance->GetPlayerLevel() + 1)->LevelUpExp;
	LevelUpCount.Init(0, AbilityArray.Num());
	OKBtnDisable();
	bUpdatedAbility = false;
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
		//�䱸 ����ġ�̻��� �����ؾ� ������ ����
			LevelUpCount[SelectUINumber]++;
			UpdateTargetExp();
			if (GetPlayerExp() < TotalExp)
			{
				LevelUpCount[SelectUINumber]--;
				UpdateTargetExp();
			}
	}
}

void ULevelUpUI::OnClickOKBtn()
{
	OKUI = Cast<UOKUI>(CreateWidget(GetWorld(), OKUIIClass));

	if (OKUI) {
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

void ULevelUpUI::UpdateTargetExp()
{
	int32 CurrentLevel = GetPlayerLevel();
	int32 TargetLevel = GetChangePlayerLevel();
	int32 dl = TargetLevel - CurrentLevel;

	bUpdatedAbility = false;
	int32 SumExp = 0;
	if (dl > 0) {
		bUpdatedAbility = true;
		OKBtnEnable();
		//���� ��ǥ���Ѵ� ������ ���� ������ �ʿ��Ѱ���ġ�� ǥ���ؾ���
		for (int i = CurrentLevel; i <= TargetLevel + 1; ++i) {
			SumExp += GameInstance->GetCharAbilityData(i)->LevelUpExp;
		}
		TotalExp = SumExp;
	}
	else
		OKBtnDisable();
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

