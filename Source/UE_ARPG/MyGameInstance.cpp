#include "MyGameInstance.h"
#include "Actor/AudioManager.h"

#include "Global.h"
#include "Sound/SoundCue.h"


UMyGameInstance::UMyGameInstance()
{
	

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


