#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_UnEquip.generated.h"


UCLASS()
class UE_ARPG_API UCAnimNotifyState_UnEquip : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	//Montage���� ����Ǵ� �̸��� �����Ѵ�.
	FString GetNotifyName_Implementation() const;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

};
