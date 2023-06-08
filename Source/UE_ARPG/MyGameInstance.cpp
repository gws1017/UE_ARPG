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
	//월드에 있는 오디오 관리자 객체를 얻어온다
	AAudioManager::GetAudioManager(GetWorld())->PlayBGM(sound);
}



void UMyGameInstance::PlayMainBGM()
{
	AAudioManager::GetAudioManager(GetWorld())->PlayMainBGM();
}


