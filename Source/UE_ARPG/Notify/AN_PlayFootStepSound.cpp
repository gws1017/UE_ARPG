#include "Notify/AN_PlayFootStepSound.h"
#include "Global.h"

#include "Material/MaterialSoundList.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

#include "Sound/SoundCue.h"

FString UAN_PlayFootStepSound::GetNotifyName_Implementation() const
{
	return "PlayFootStepSound";
}

void UAN_PlayFootStepSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(Character);

	FVector Start = Character->GetActorLocation();//GetMesh()->GetSocketLocation(BoneName);
	FVector End = Start;
	End.Z -= TraceDistance;

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Character);
	
	FHitResult HitResult;
	bool result = UKismetSystemLibrary::LineTraceSingle(
		Character->GetWorld(),
		Start, End, 
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		false, 
		IgnoreActors, 
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResult,
		true);

	if (result)
	{
		EPhysicalSurface SurfaceType =  UGameplayStatics::GetSurfaceType(HitResult);
		if (BPSoundList && SurfaceType)
		{
			AMaterialSoundList* SoundList = Cast<AMaterialSoundList>(BPSoundList->GetDefaultObject());
			UGameplayStatics::PlaySoundAtLocation(
				Character->GetWorld(),
				SoundList->GetSound(SurfaceType),
				Start,
				VoulmeMultiplier
			);
		}
	}
}
