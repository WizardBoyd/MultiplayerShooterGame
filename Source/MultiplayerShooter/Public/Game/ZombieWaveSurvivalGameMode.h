// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZombieWaveSurvivalGameMode.generated.h"

class AZombieWaveSurvivalPlayerSpawnPoint;

UCLASS()
class MULTIPLAYERSHOOTER_API AZombieWaveSurvivalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AZombieWaveSurvivalGameMode();

protected:
	bool bHasloadedSpawnPoints;
	TArray<TObjectPtr<AZombieWaveSurvivalPlayerSpawnPoint>> PlayerSpawnPoints;

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void SetSpawnPoints();
};
