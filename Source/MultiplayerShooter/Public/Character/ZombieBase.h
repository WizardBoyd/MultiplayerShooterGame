﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieBase.generated.h"
class AZombieWaveSurvivalCharacter;
UCLASS()
class MULTIPLAYERSHOOTER_API AZombieBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int8 CalculatePointsForKill(const FString& BoneName);


public:
	void Hit(AZombieWaveSurvivalCharacter* Player, const FHitResult& HitResult);

};
