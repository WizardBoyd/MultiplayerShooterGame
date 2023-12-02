// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/MultiplayerZombieMainMenuGameMode.h"
#include "OnlineBeaconHost.h"
#include "MultiplayerShooter/Public/OnlineSystem/ZombieBeaconHostObject.h"


AMultiplayerZombieMainMenuGameMode::AMultiplayerZombieMainMenuGameMode()
{
	HostObject = nullptr;
	Host = nullptr;
}

bool AMultiplayerZombieMainMenuGameMode::CreateHostBeacon()
{
	UE_LOG(LogTemp, Warning, TEXT("Called CreateHostBeacon"))
	if((Host = GetWorld()->SpawnActor<AOnlineBeaconHost>(AOnlineBeaconHost::StaticClass())))
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawned A Online Beacon Host"))
		if(Host->InitHost())
		{
			UE_LOG(LogTemp, Warning, TEXT("Init A Online Beacon Host"))
			Host->PauseBeaconRequests(false);
			
			if((HostObject = GetWorld()->SpawnActor<AZombieBeaconHostObject>(AZombieBeaconHostObject::StaticClass())))
			{
				UE_LOG(LogTemp, Warning, TEXT("Spawned A Zombie Online Beacon Host"))
				Host->RegisterHost(HostObject);
				return true;
			}
		}
	}
	return false;
}

AZombieBeaconHostObject* AMultiplayerZombieMainMenuGameMode::GetBeaconHost()
{
	return HostObject;
}



