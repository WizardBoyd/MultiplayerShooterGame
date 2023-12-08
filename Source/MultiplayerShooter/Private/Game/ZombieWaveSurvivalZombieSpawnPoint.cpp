// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZombieWaveSurvivalZombieSpawnPoint.h"
#include "Useables/Barricade.h"


// Sets default values
AZombieWaveSurvivalZombieSpawnPoint::AZombieWaveSurvivalZombieSpawnPoint()
{
	Zone = 0;
	bIsActive = false;
}

void AZombieWaveSurvivalZombieSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	
}

ABarricade* AZombieWaveSurvivalZombieSpawnPoint::GetLinkedBarricade()
{
	return LinkedBarricade;
}

uint8 AZombieWaveSurvivalZombieSpawnPoint::GetZone()
{
	return Zone;
}

void AZombieWaveSurvivalZombieSpawnPoint::SetZone(uint8 ZoneNumber)
{
	Zone = ZoneNumber;
}

void AZombieWaveSurvivalZombieSpawnPoint::Activate()
{
	bIsActive = true;
}

bool AZombieWaveSurvivalZombieSpawnPoint::IsActive()
{
	return bIsActive;
}


