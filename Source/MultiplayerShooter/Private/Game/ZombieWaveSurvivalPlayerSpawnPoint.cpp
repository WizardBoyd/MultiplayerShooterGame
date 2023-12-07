// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZombieWaveSurvivalPlayerSpawnPoint.h"


// Sets default values
AZombieWaveSurvivalPlayerSpawnPoint::AZombieWaveSurvivalPlayerSpawnPoint()
{
	bIsUsed = false;
}

bool AZombieWaveSurvivalPlayerSpawnPoint::IsUsed()
{
	return bIsUsed;
}

void AZombieWaveSurvivalPlayerSpawnPoint::SetUsed(bool Used)
{
	bIsUsed = Used;
}



