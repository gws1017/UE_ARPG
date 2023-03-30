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
	if (!AM)
	{
		TArray<AAudioManager*> arr;
		UHelpers::FindActors(GetWorld(), arr);
		AM = arr[0];
	}

	AM->PlayBGM(sound);
}


