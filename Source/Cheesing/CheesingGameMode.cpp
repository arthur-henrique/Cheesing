// Copyright Epic Games, Inc. All Rights Reserved.

#include "CheesingGameMode.h"
#include "CheesingCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACheesingGameMode::ACheesingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
