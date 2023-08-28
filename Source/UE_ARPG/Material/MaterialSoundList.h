#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MaterialSoundList.generated.h"
class USoundCue;

UCLASS()
class UE_ARPG_API AMaterialSoundList : public AActor
{
	GENERATED_BODY()
	
public:	
	AMaterialSoundList();

public:
	 
	UPROPERTY(EditDefaultsOnly)
		TMap<TEnumAsByte<EPhysicalSurface>,USoundCue*> SoundList;

	UFUNCTION()
		USoundCue* GetSound(EPhysicalSurface SurfaceType) { return SoundList[SurfaceType]; }
};
