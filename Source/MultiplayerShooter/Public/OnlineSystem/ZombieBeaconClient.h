// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "ZombieBeaconClient.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API AZombieBeaconClient : public AOnlineBeaconClient
{
	GENERATED_BODY()

public:
	AZombieBeaconClient();
	
};
