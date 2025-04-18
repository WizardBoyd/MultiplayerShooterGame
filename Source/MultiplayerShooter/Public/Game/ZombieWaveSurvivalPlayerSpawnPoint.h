﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "ZombieWaveSurvivalPlayerSpawnPoint.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API AZombieWaveSurvivalPlayerSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZombieWaveSurvivalPlayerSpawnPoint();

protected:
	bool bIsUsed;

public:
	bool IsUsed();
	void SetUsed(bool Used);
};
