// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineSystem/ZombieBeaconHostObject.h"

#include "OnlineBeaconHost.h"
#include "OnlineSystem/ZombieBeaconClient.h"

AZombieBeaconHostObject::AZombieBeaconHostObject()
{
	ClientBeaconActorClass = AZombieBeaconClient::StaticClass();
	BeaconTypeName = ClientBeaconActorClass->GetName();
}

void AZombieBeaconHostObject::OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection)
{
	Super::OnClientConnected(NewClientActor, ClientConnection);

	if(NewClientActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Connected Client Valid"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Connected Client InValid"))
	}
}

void AZombieBeaconHostObject::NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor)
{
	Super::NotifyClientDisconnected(LeavingClientActor);
	UE_LOG(LogTemp, Warning, TEXT("Client Has Disconnected"))
}

void AZombieBeaconHostObject::ShutdownServer()
{
	//Unregister Server from database via web api
	DisconnectAllClients();
	if(AOnlineBeaconHost* Host = Cast<AOnlineBeaconHost>(GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying Host Beacon"))
		Host->UnregisterHost(BeaconTypeName);
		Host->DestroyBeacon();
	}
}

void AZombieBeaconHostObject::DisconnectAllClients()
{
	UE_LOG(LogTemp, Warning, TEXT("Disconnecting All Clients"))
	for(AOnlineBeaconClient* Client: ClientActors)
	{
		if(Client)
			DisconnectClient(Client);
	}
}

void AZombieBeaconHostObject::DisconnectClient(AOnlineBeaconClient* ClientActor)
{
	AOnlineBeaconHost* BeaconHost = Cast<AOnlineBeaconHost>(GetOwner());
	if(BeaconHost)
	{
		if(AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(ClientActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("DISCONNECTIN CLIENT %s"), *ClientActor->GetName())
			Client->Client_OnDisconnected();
		}
		BeaconHost->DisconnectClient(ClientActor);
	}
}
