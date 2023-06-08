#include "Actor/AudioManager.h"

#include "Global.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

AAudioManager* AAudioManager::AM = nullptr;
// Sets default values
AAudioManager::AAudioManager()
{
	AM = this;
	UHelpers::CreateComponent(this, &MainAudio, "MainAudio");

	UHelpers::GetAsset(&MainBGM, "SoundCue'/Game/Audio/dungeon002_Cue.dungeon002_Cue'");

	MainAudio->SetSound(MainBGM);

}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();
	
	MainAudio->Play();
}

void AAudioManager::PlayBGM(USoundCue* sound)
{
	CheckNull(sound);
	CheckNull(AM);
	MainAudio->Stop();

	MainAudio->SetSound(sound);
	MainAudio->Play();

}

void AAudioManager::PlayMainBGM()
{
	CheckNull(AM);
	MainAudio->SetSound(MainBGM);
	MainAudio->Play();
}

AAudioManager* AAudioManager::GetAudioManager(UWorld* world)
{
	if (!AM)
	{
		TArray<AAudioManager*> arr;
		UHelpers::FindActors(world, arr);
		AM = arr[0];
	}
	return AM;
}

