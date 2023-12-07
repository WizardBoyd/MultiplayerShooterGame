// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameModes/MultiplayerShooterGameModeBase.h"

#include "UI/HUD/ZombieSurvivalHUD.h"

AMultiplayerShooterGameModeBase::AMultiplayerShooterGameModeBase()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFiner(TEXT("/Game/Blueprints/Player/BP_CharacterBase.BP_CharacterBase_C"));
	DefaultPawnClass = PlayerPawnClassFiner.Class;

	HUDClass = AZombieSurvivalHUD::StaticClass();
}

void AMultiplayerShooterGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("Post Login GAMEMODE"))
}

void AMultiplayerShooterGameModeBase::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("BEGIN PLAY GAMEMODE"))
}
