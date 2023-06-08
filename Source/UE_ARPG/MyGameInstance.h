// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:


public:
	UMyGameInstance();

	virtual void Init() override;

	void PlayBGM(class USoundCue* sound);
	void PlayMainBGM();

};
