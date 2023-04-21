// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestLocatorGameMode.h"
#include "QuestLocatorCharacter.h"
#include "UObject/ConstructorHelpers.h"

AQuestLocatorGameMode::AQuestLocatorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
