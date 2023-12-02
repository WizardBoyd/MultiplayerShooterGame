// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerZombieMainMenuGameMode.generated.h"

class AZombieBeaconHostObject;
class AOnlineBeaconHost;

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerZombieMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiplayerZombieMainMenuGameMode();

protected:
	UFUNCTION(BlueprintCallable)
	bool CreateHostBeacon();

	TObjectPtr<AZombieBeaconHostObject> HostObject;
	TObjectPtr<AOnlineBeaconHost> Host;

	UFUNCTION(BlueprintCallable)
	AZombieBeaconHostObject* GetBeaconHost();
	
};
