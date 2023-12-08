// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZombieBase.h"

#include "Character/ZombieWaveSurvivalCharacter.h"


AZombieBase::AZombieBase()
{
	
}


void AZombieBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZombieBase::Hit(AZombieWaveSurvivalCharacter* Player)
{
	if(HasAuthority() && Player)
	{
		Player->IncrementPoints(100);
	}
}


