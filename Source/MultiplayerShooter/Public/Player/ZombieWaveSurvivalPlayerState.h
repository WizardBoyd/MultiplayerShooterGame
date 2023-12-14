// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ZombieWaveSurvivalPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPointsChanged, int32, OnPointsChanged);

UCLASS()
class MULTIPLAYERSHOOTER_API AZombieWaveSurvivalPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AZombieWaveSurvivalPlayerState();

protected:

	UPROPERTY(BlueprintAssignable)
	FPointsChanged OnPointsChanged;
	
	UPROPERTY(ReplicatedUsing = OnRep_PointsChanged ,EditDefaultsOnly) //Set to replicate
	int32 Points;

	UFUNCTION()
	void OnRep_PointsChanged();

public:
	void IncrementPoints(uint16 Value);
	bool DecrementPoints(uint16 Value);
	UFUNCTION(BlueprintCallable)
	int32 GetPoints();

};
