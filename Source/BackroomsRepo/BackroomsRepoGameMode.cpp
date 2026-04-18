// Copyright Epic Games, Inc. All Rights Reserved.

#include "BackroomsRepoGameMode.h"
#include "BackroomsRepoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABackroomsRepoGameMode::ABackroomsRepoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/MAIN/FP/FirstPerson/Blueprints/BP_FirstPersonCharacter.BP_FirstPersonCharacter_C"));

	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
