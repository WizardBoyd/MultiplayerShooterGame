// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZombieWaveSurvivalGameMode.generated.h"

class AZombieWaveSurvivalCharacter;
class AZombieSurvivalGameState;
class AZombieWaveSurvivalZombieSpawnPoint;
class AZombieBase;
class AZombieWaveSurvivalPlayerSpawnPoint;

UCLASS()
class MULTIPLAYERSHOOTER_API AZombieWaveSurvivalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AZombieWaveSurvivalGameMode();

protected:

	TObjectPtr<AZombieSurvivalGameState> ZombieGameState;
	
	bool bHasloadedSpawnPoints;
	TArray<TObjectPtr<AZombieWaveSurvivalPlayerSpawnPoint>> PlayerSpawnPoints;

	TArray<TObjectPtr<AZombieWaveSurvivalZombieSpawnPoint>> ZombieSpawnPoints;

	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
	TSubclassOf<AZombieBase> ZombieClass;

	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
	TSubclassOf<AZombieWaveSurvivalCharacter> PlayerClass;

	FTimerHandle TZombieSpawnHandle;

	//Max Zombies on map is 24 + (PlayerCount * 6) this is for any one time

	//zombies per round is multiplier * max zombies on map. (the multiplier is the round number * 0.15)
	//example round 10 with 3 players is (multiplier = 10*0.15) = 1.5 * 36 (24 + Playercount*6) = 54 TOTAL ZOMBIES ON ROUND 10 WITH 3 Players

	uint16 ZombiesRemaining;
	
protected:
	void SpawnZombie();
	

protected:

	void CalculateZombieCount();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void SetSpawnPoints();

	virtual void BeginPlay() override;
};
