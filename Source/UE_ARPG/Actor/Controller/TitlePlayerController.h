// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitlePlayerController.generated.h"

class UTitleUI;
UCLASS()
class UE_ARPG_API ATitlePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "UI")
		UTitleUI* TitleUI;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		TSubclassOf<UTitleUI> TitleUIClass;

public:

	ATitlePlayerController();

public:

	virtual void BeginPlay() override;
};
