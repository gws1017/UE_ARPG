#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState_Trail.h"
#include "CAnimNotifyState_SwordTrail.generated.h"

UCLASS()
class UE_ARPG_API UCAnimNotifyState_SwordTrail : public UAnimNotifyState_Trail
{
	GENERATED_BODY()
public:
	UCAnimNotifyState_SwordTrail(const FObjectInitializer& ObjectInitializer);
public:
	FString GetNotifyName_Implementation() const;

};
