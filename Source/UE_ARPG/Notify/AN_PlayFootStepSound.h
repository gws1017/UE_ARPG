#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_PlayFootStepSound.generated.h"

class AMaterialSoundList;

UCLASS()
class UE_ARPG_API UAN_PlayFootStepSound : public UAnimNotify
{
	GENERATED_BODY()

public:

	FString GetNotifyName_Implementation() const;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
		const FAnimNotifyEventReference& EventReference);

	UPROPERTY(EditAnywhere)
		float VoulmeMultiplier = 0.5f;
	UPROPERTY(EditAnywhere)
		float TraceDistance = 100.f;

	UPROPERTY(EditAnywhere)
		bool bShowBoxDebug = false;

	UPROPERTY(EditAnywhere)
		FName BoneName;
	UPROPERTY(EditAnywhere)
	class TSubclassOf<AMaterialSoundList> BPSoundList;
};
