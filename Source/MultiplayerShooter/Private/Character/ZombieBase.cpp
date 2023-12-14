// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZombieBase.h"
#include "Player/ZombieWaveSurvivalPlayerState.h"
#include "Character/ZombieWaveSurvivalCharacter.h"


AZombieBase::AZombieBase()
{
	
}


void AZombieBase::BeginPlay()
{
	Super::BeginPlay();
	
}

int8 AZombieBase::CalculatePointsForKill(const FString& BoneName)
{

	if(BoneName.Contains(FString("l")) || BoneName.Contains(FString("r")))
	{
		//Limb Hit
		UE_LOG(LogTemp, Warning, TEXT("KILLED WITH LIMB SHOT"));
		return 50;
	}
	else if(BoneName.Contains(FString("spine")))
	{
		UE_LOG(LogTemp, Warning, TEXT("KILLED WITH TORSO SHOT"));
		return 60;
		
	}else if(BoneName.Contains(FString("neck")))
	{
		UE_LOG(LogTemp, Warning, TEXT("KILLED WITH NECK SHOT"));
		return 70;
	}else if(BoneName.Contains(FString("head")))
	{
		UE_LOG(LogTemp, Warning, TEXT("KILLED WITH HEAD SHOT"));
		return 100;
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("JUST KILLED NO WHERE SPECIAL"));
		return 20;
	}
}

void AZombieBase::Hit(AZombieWaveSurvivalCharacter* Player, const FHitResult& HitResult)
{
	if(Player)
	{
		if(AZombieWaveSurvivalPlayerState* PState = Player->GetPlayerState<AZombieWaveSurvivalPlayerState>())
		{
			FString BoneName = HitResult.BoneName.ToString();
			if(BoneName == FString("None"))
				return;
			PState->IncrementPoints(CalculatePointsForKill(BoneName));
		}
	}
}


