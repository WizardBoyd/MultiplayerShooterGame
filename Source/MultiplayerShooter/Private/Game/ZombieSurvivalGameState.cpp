// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZombieSurvivalGameState.h"


// Sets default values
AZombieSurvivalGameState::AZombieSurvivalGameState()
{
	RoundNumber = 1;
	ZombiesRemainingOnMap = 0;
}

uint16 AZombieSurvivalGameState::GetRoundNumber()
{
	return RoundNumber;
}

void AZombieSurvivalGameState::IncrementRoundNumber()
{
	++RoundNumber;
}


