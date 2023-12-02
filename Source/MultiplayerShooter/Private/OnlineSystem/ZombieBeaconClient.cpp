// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineSystem/ZombieBeaconClient.h"

AZombieBeaconClient::AZombieBeaconClient()
{
}

bool AZombieBeaconClient::ConnectToServer(const FString& Address)
{
	FURL Destination = FURL(nullptr, *Address, TRAVEL_Absolute);
	Destination.Port = 7787; //Port from config. should try to make variable maybe
	return InitClient(Destination);
}

void AZombieBeaconClient::LeaveLobby()
{
	DestroyBeacon();
}

void AZombieBeaconClient::OnFailure()
{
	Super::OnFailure();
	UE_LOG(LogTemp, Warning, TEXT("CLIENT FAILED TO CONNECT TO HOST BEACON"))
	OnClientConnected.Broadcast(false);
}

void AZombieBeaconClient::OnConnected()
{
	Super::OnConnected();
	UE_LOG(LogTemp, Warning, TEXT("CLIENT CONNECTED TO HOST BEACON"))
	OnClientConnected.Broadcast(true);
}

void AZombieBeaconClient::Client_OnDisconnected_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("DISCONNECTED"))
	OnClientDisconnected.Broadcast();
}



