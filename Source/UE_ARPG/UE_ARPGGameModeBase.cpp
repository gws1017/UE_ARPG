// Copyright Epic Games, Inc. All Rights Reserved.


#include "UE_ARPGGameModeBase.h"
#include "Global.h"

AUE_ARPGGameModeBase::AUE_ARPGGameModeBase()
{
	UHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Character/BP_CPlayer.BP_CPlayer_C'");;
}