// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ZombieSurvivalGameState.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API AZombieSurvivalGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZombieSurvivalGameState();

protected:
	uint16 RoundNumber; //technically this means there is an upper limit of rounds highest uint 16 number
	uint16 ZombiesRemainingOnMap;


public:
	uint16 GetRoundNumber();
	void IncrementRoundNumber();
};
