#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_DeathEnd.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API UCAnimNotify_DeathEnd : public UAnimNotify
{
	GENERATED_BODY()

public:

	FString GetNotifyName_Implementation() const;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
};
