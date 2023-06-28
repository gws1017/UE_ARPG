#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteraction.generated.h"

UINTERFACE(MinimalAPI)
class UInteraction : public UInterface
{
	GENERATED_BODY()
};

class UE_ARPG_API IInteraction
{
	GENERATED_BODY()

public:

	virtual void OnInteraction() = 0;
};
