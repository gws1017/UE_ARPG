#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance.generated.h"

UCLASS()
class UE_ARPG_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Animation")
		float Speed;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Animation")
		float Direction;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Animation")
		bool bEquipped;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Enemy | Animation")
		bool bAlert;
public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY()
		class ACharacter* OwnerCharacter;
};
