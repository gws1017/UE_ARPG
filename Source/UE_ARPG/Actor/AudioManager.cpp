#include "Actor/AudioManager.h"

#include "Global.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AAudioManager::AAudioManager()
{
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

	MainAudio->Stop();

	MainAudio->SetSound(sound);
	MainAudio->Play();

}

