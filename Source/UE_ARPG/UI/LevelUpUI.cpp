#include "UI/LevelUpUI.h"
#include "Actor/CPlayer.h"
#include "Global.h"

void ULevelUpUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	PlayerInstance = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

