#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

USTRUCT(BlueprintType)
struct FSaveData {
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		struct FPlayerStatus Status;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		FVector Location;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		FRotator Rotation;

};

UCLASS()
class UE_ARPG_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UMySaveGame();

	UPROPERTY(VisibleAnywhere , Category = "Basic")
		FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
		uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
		FSaveData SaveData;
};
