// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AudioManager.generated.h"

UCLASS()
class UE_ARPG_API AAudioManager : public AActor
{
	GENERATED_BODY()
	
private:

	static AAudioManager* AM;

	UPROPERTY(VisibleDefaultsOnly, Category = "Sound")
		class UAudioComponent* MainAudio;

	UPROPERTY(VisibleDefaultsOnly, Category = "Sound")
		class USoundCue* MainBGM;
public:	
	// Sets default values for this actor's properties
	AAudioManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void PlayBGM(class USoundCue* sound);
	void PlayMainBGM( );

public:
	static AAudioManager* GetAudioManager(UWorld* world);

};
