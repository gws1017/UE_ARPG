#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

USTRUCT(BlueprintType)
struct FSaveData {
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere,Category = "SaveData")
		float HP;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		float MaxHP;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		float Stamina;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		float MaxStamina;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		float StrDamage;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		int32 Vigor;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		int32 Enduarance;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		int32 Strength;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		int32 Level;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		int32 Exp;

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
