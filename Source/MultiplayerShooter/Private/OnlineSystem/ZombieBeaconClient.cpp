// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineSystem/ZombieBeaconClient.h"

AZombieBeaconClient::AZombieBeaconClient()
{
	PlayerIndex = 0;
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

void AZombieBeaconClient::SendChatMessage(FText ChatMessage)
{
	Server_SendChatMessage(ChatMessage);
}

void AZombieBeaconClient::Server_SendChatMessage_Implementation(const FText& ChatMessage)
{
	FString Message = PlayerName + ": " + ChatMessage.ToString();
	UE_LOG(LogTemp, Warning, TEXT("Chat: %s"), *Message);
	if(AZombieBeaconHostObject* Host = Cast<AZombieBeaconHostObject>(BeaconOwner))
	{
		Host->SendChatToLobby(FText::FromString(Message));
	}
}

bool AZombieBeaconClient::Server_SendChatMessage_Validate(const FText& ChatMessage)
{
	return true;
}

void AZombieBeaconClient::Client_OnDisconnected_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("DISCONNECTED"))
	OnClientDisconnected.Broadcast();
}

void AZombieBeaconClient::Client_OnChatMessageReceived_Implementation(const FText& ChatMessage)
{
	OnChatReceived.Broadcast(ChatMessage);
}

void AZombieBeaconClient::Client_OnLobbyUpdated_Implementation(FZombieLobbyInfo LobbyInfo)
{
	OnLobbyUpdated.Broadcast(LobbyInfo);
}

void AZombieBeaconClient::SetPlayerIndex(uint8 Index)
{
	PlayerIndex = Index;
}

uint8 AZombieBeaconClient::GetPlayerIndex()
{
	return PlayerIndex;
}

void AZombieBeaconClient::SetPlayerName(const FString& NewPlayerName)
{
	PlayerName = NewPlayerName;
}

FString AZombieBeaconClient::GetPlayerName()
{
	return PlayerName;
}



