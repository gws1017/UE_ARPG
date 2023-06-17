#include "MyGameInstance.h"
#include "Actor/AudioManager.h"
#include "DataTable/CharacterAbilityTables.h"

#include "Global.h"
#include "Sound/SoundCue.h"


UMyGameInstance::UMyGameInstance()
{
	
	//GetAsset<UDataTable>(&CharacterAbilityData, "");
}

void UMyGameInstance::Init()
{
	Super::Init();

}

void UMyGameInstance::PlayBGM(USoundCue* sound)
{
	//���忡 �ִ� ����� ������ ��ü�� ���´�
	AAudioManager::GetAudioManager(GetWorld())->PlayBGM(sound);
}



void UMyGameInstance::PlayMainBGM()
{
	AAudioManager::GetAudioManager(GetWorld())->PlayMainBGM();
}

FCharacterAbilityData* UMyGameInstance::GetCharAbilityData(int32 Level)
{
	return CharacterAbilityData->FindRow<FCharacterAbilityData>(*FString::FromInt(Level),TEXT(""));
}


