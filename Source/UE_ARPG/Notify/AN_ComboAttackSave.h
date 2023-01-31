// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_ComboAttackSave.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API UAN_ComboAttackSave : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	FString GetNotifyName_Implementation() const;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
};
