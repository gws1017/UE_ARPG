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
	TargetExp = PlayerInstance->GetLevelUpExp();
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
	//혹시 모르니까 한번더 확인, 변경된 항목만 적용한다
	
	if (PlayerInstance->GetMaxHP() != GetChangeMaxHP())PlayerInstance->SetMaxHP(GetChangeMaxHP());
	if (PlayerInstance->GetMaxStamina() != GetChangeMaxStamina())PlayerInstance->SetMaxStamina(GetChangeMaxStamina());
	if (PlayerInstance->GetDamage() != GetChangePlayerDamage())PlayerInstance->SetStrDamage(GameInstance->GetCharAbilityData(GetChangeStrength())->TotalDmgIncrease);
	
	if (PlayerInstance->GetExp() != GetChangePlayerExp())PlayerInstance->SetExp(GetChangePlayerExp());
	if (PlayerInstance->GetPlayerLevel() != GetChangePlayerLevel())PlayerInstance->SetPlayerLevel(GetChangePlayerLevel());
	if (PlayerInstance->GetVigor() != GetChangeVigor())PlayerInstance->SetVigor(GetChangeVigor());
	if (PlayerInstance->GetEnduarance() != GetChangeEnduarance())PlayerInstance->SetEnduarance(GetChangeEnduarance());
	if (PlayerInstance->GetStrength() != GetChangeStrength())PlayerInstance->SetStrength(GetChangeStrength());
	if (PlayerInstance->GetLevelUpExp() != TargetExp) PlayerInstance->SetLevelUpExp(TargetExp);
	TargetExp = PlayerInstance->GetLevelUpExp();
	LevelUpCount.Init(0, AbilityArray.Num());
	//최종 무기데미지 != 바뀐 최종무기데미지) 스텟추가데미지 = 바뀐 힘스텟에 의한 누적추가데미지
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

	TargetExp = PlayerInstance->GetLevelUpExp();
	int32 SumExp = TargetExp;
	if (dl > 0) {
		OKBtnEnable();
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
	if (GameInstance)
	{
		//추가 hp(2레벨) + 초기체력
		//추가hp(3레벨) + 추가 hp(2레벨) + 초기체력 -> 추가HP(3레벨) + 초기 체력
		//계산식이 현재 잘못되어있다, 스테미나도 같은문제 발생
		//해결 
		//1.계산된 체력을 가져와서 그대로 대입한다
		//2.추가체력을 저장하는 변수를 만든다(변수너무많고 귀찮을거같은데)
		//3.또뭐가잇지
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

