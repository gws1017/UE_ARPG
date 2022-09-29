#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

UINTERFACE(MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

class UE_ARPG_API IICharacter
{
	GENERATED_BODY()

public:

	virtual class AWeapon* GetWeapon() = 0;

	virtual void DeathEnd() = 0;
};
