// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "ZombieWaveSurvivalZombieSpawnPoint.generated.h"

class ABarricade;
UCLASS()
class MULTIPLAYERSHOOTER_API AZombieWaveSurvivalZombieSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZombieWaveSurvivalZombieSpawnPoint();

protected:
	UPROPERTY(EditAnywhere, Category = "Zombie Wave Survival Settings")
		TObjectPtr<ABarricade> LinkedBarricade;

	uint8 Zone;
	bool bIsActive;

	virtual void BeginPlay() override;

public:
	ABarricade* GetLinkedBarricade();
	uint8 GetZone();
	void SetZone(uint8 ZoneNumber);
	void Activate();
	bool IsActive();
};
