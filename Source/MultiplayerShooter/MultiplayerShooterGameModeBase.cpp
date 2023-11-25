// Copyright Epic Games, Inc. All Rights Reserved.


#include "MultiplayerShooterGameModeBase.h"
#include "UI/HUD/ZombieSurvivalHUD.h"

AMultiplayerShooterGameModeBase::AMultiplayerShooterGameModeBase()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFiner(TEXT("/Game/Blueprints/Player/BP_CharacterBase.BP_CharacterBase_C"));
	DefaultPawnClass = PlayerPawnClassFiner.Class;

	HUDClass = AZombieSurvivalHUD::StaticClass();
}
