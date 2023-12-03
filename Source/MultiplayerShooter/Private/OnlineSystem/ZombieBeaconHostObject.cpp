// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineSystem/ZombieBeaconHostObject.h"

#include "OnlineBeaconHost.h"
#include "OnlineSystem/ZombieBeaconClient.h"

AZombieBeaconHostObject::AZombieBeaconHostObject()
{
	ClientBeaconActorClass = AZombieBeaconClient::StaticClass();
	BeaconTypeName = ClientBeaconActorClass->GetName();
}

void AZombieBeaconHostObject::UpdateLobbyInfo(FZombieLobbyInfo NewLobbyInfo)
{
	LobbyInfo.MapImage = NewLobbyInfo.MapImage;
	UpdateClientLobbyInfo();
	OnLobbyUpdated.Broadcast(LobbyInfo);
}

void AZombieBeaconHostObject::UpdateClientLobbyInfo()
{
	for(AOnlineBeaconClient* ClientBeacon: ClientActors)
	{
		if(AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(ClientBeacon))
		{
			Client->Client_OnLobbyUpdated(LobbyInfo);
		}
	}
}

void AZombieBeaconHostObject::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("BEACON BEGIN PLAY"))
	LobbyInfo.PlayerList.Add(FString("Host"));
}

void AZombieBeaconHostObject::OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection)
{
	Super::OnClientConnected(NewClientActor, ClientConnection);

	if(NewClientActor)
	{
		FString PlayerName = FString("Player ");
		const uint8 Index = LobbyInfo.PlayerList.Num();
		PlayerName.Append(FString::FromInt(Index));
		LobbyInfo.PlayerList.Add(PlayerName);

		if(AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(NewClientActor))
		{
			Client->SetPlayerIndex(Index);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Connected Client Valid"))
		OnLobbyUpdated.Broadcast(LobbyInfo);
		UpdateClientLobbyInfo();
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

	// if(AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(LeavingClientActor))
	// {
	// 	uint8 Index = Client->GetPlayerIndex();
	// 	LobbyInfo.PlayerList.RemoveAt(Index);
	// }
	ReAssignPlayerIDs();
	OnLobbyUpdated.Broadcast(LobbyInfo);
	UpdateClientLobbyInfo();
}

void AZombieBeaconHostObject::ShutdownServer()
{
	UE_LOG(LogTemp, Warning, TEXT("ATTEMPTING SHUT DOWN SERVER"))
	//Unregister Server from database via web api
	DisconnectAllClients();
	if(AOnlineBeaconHost* Host = Cast<AOnlineBeaconHost>(GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying Host Beacon"))
		Host->UnregisterHost(BeaconTypeName);
		Host->DestroyBeacon();
	}
	UE_LOG(LogTemp, Warning, TEXT("SERVER SHUT DOWN"))
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

void AZombieBeaconHostObject::ReAssignPlayerIDs()
{
	LobbyInfo.PlayerList.Empty();
	LobbyInfo.PlayerList.Add(FString("Host"));
	for(AOnlineBeaconClient* ClientBeacon: ClientActors)
	{
		if(AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(ClientBeacon))
		{
			FString PlayerName = FString("Player ");
			const uint8 Index = LobbyInfo.PlayerList.Num();
			PlayerName.Append(FString::FromInt(Index));
			LobbyInfo.PlayerList.Add(PlayerName);
			Client->SetPlayerIndex(Index);
		}
	}
}
