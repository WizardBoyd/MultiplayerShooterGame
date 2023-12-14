// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZombieWaveSurvivalPlayerState.h"
#include "Net/UnrealNetwork.h"

AZombieWaveSurvivalPlayerState::AZombieWaveSurvivalPlayerState()
{
	Points = 500;
}
void AZombieWaveSurvivalPlayerState::OnRep_PointsChanged()
{
	OnPointsChanged.Broadcast(Points);
}
void AZombieWaveSurvivalPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZombieWaveSurvivalPlayerState, Points);
}
void AZombieWaveSurvivalPlayerState::IncrementPoints(uint16 Value)
{
	if(HasAuthority())
	{
		Points += Value;
		OnRep_PointsChanged();
		UE_LOG(LogTemp, Warning, TEXT("Points have Increased On %s : %d"),*GetName(),Points);
	}
}
bool AZombieWaveSurvivalPlayerState::DecrementPoints(uint16 Value)
{
	if(HasAuthority())
	{
		if((Points - Value) < 0)
		{
			return false;
		}else
		{
			Points -= Value;
			OnRep_PointsChanged();
			UE_LOG(LogTemp, Warning, TEXT("Points have Decresed On %s : %d"),*GetName(),Points);
			return true;
		}
	}
	return true;
}
int32 AZombieWaveSurvivalPlayerState::GetPoints()
{
	return Points;
}
