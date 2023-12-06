// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineSystem/ZombieBeaconHostObject.h"
#include "OnlineBeaconHost.h"
#include "OnlineSystem/ZombieBeaconClient.h"
#include "TimerManager.h"

AZombieBeaconHostObject::AZombieBeaconHostObject()
{
	ClientBeaconActorClass = AZombieBeaconClient::StaticClass();
	BeaconTypeName = ClientBeaconActorClass->GetName();
	ServerId = -1;
	Http = &FHttpModule::Get();
}

void AZombieBeaconHostObject::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
	if(Success)
	{
		ServerId = FCString::Atoi(*Response->GetContentAsString());
		UE_LOG(LogTemp, Warning, TEXT("Sucess ID: %d"), ServerId);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HttpRequest Failed: %s"), *Response->GetContentAsString());
	}
}

void AZombieBeaconHostObject::SetServerData(const FServerData& NewServerData)
{
	UE_LOG(LogTemp, Warning, TEXT("SetServerData CALLED"))
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	ServerData = NewServerData;
	ServerData.CurrentPlayers = GetCurrentPlayersCount();
	JsonObject->SetNumberField("ServerId",ServerData.ServerID);
	JsonObject->SetStringField("IPAddress",ServerData.IPAddress.IsEmpty() ? "127.0.0.1" : ServerData.IPAddress);
	JsonObject->SetStringField("ServerName",ServerData.ServerName.IsEmpty() ? FString("DefaultServerName") : ServerData.ServerName);
	JsonObject->SetStringField("MapName",ServerData.MapName.IsEmpty() ? FString("DefaultMapName") : ServerData.MapName);
	JsonObject->SetNumberField("CurrentPlayers", ServerData.CurrentPlayers);
	JsonObject->SetNumberField("MaxPlayers", ServerData.MaxPlayers);

	FString JsonString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnProcessRequestComplete);
	
	Request->SetURL("https://localhost:44364/api/host");
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	Request->SetContentAsString(JsonString);

	Request->ProcessRequest();
}

void AZombieBeaconHostObject::UpdateServerData(const FServerData& NewServerData)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	ServerData = NewServerData;
	ServerData.CurrentPlayers = GetCurrentPlayersCount();
	JsonObject->SetNumberField("ServerId",ServerData.ServerID);
	JsonObject->SetStringField("IPAddress",ServerData.IPAddress.IsEmpty() ? "127.0.0.1" : ServerData.IPAddress);
	JsonObject->SetStringField("ServerName",ServerData.ServerName.IsEmpty() ? FString("DefaultServerName") : ServerData.ServerName);
	JsonObject->SetStringField("MapName",ServerData.MapName.IsEmpty() ? FString("DefaultMapName") : ServerData.MapName);
	JsonObject->SetNumberField("CurrentPlayers", ServerData.CurrentPlayers);
	JsonObject->SetNumberField("MaxPlayers", ServerData.MaxPlayers);

	FString JsonString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnProcessRequestComplete);
	
	Request->SetURL("https://localhost:44364/api/host/1");
	Request->SetVerb("PUT");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	Request->SetContentAsString(JsonString);

	Request->ProcessRequest();
}

int AZombieBeaconHostObject::GetCurrentPlayersCount()
{
	return LobbyInfo.PlayerList.Num();
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

void AZombieBeaconHostObject::InitialLobbyHandling()
{
	UE_LOG(LogTemp, Warning, TEXT("InitialLobbyHandling CALLED"))
	UpdateLobbyInfo(LobbyInfo);

	// TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	// JsonObject->SetNumberField("ServerId",0);
	// JsonObject->SetStringField("IPAddress","127.0.0.1");
	// JsonObject->SetStringField("ServerName","Test Server Name");
	// JsonObject->SetStringField("MapName","Test Map Name");
	// JsonObject->SetNumberField("CurrentPlayers",1);
	// JsonObject->SetNumberField("MaxPlayers",5);
	//
	// FString JsonString;
	// TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	// FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
	//
	// TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	//
	// Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnProcessRequestComplete);
	//
	// Request->SetURL("https://localhost:44364/api/host");
	// Request->SetVerb("POST");
	// Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	//
	// Request->SetContentAsString(JsonString);
	//
	// Request->ProcessRequest();
}

void AZombieBeaconHostObject::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("BEACON BEGIN PLAY"))
	LobbyInfo.PlayerList.Add(FString("Host"));
	GetWorld()->GetTimerManager().SetTimer(TInitialLobbyHandle, this, &ThisClass::InitialLobbyHandling, 0.2f, false);
}

void AZombieBeaconHostObject::OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection)
{
	Super::OnClientConnected(NewClientActor, ClientConnection);

	if(NewClientActor)
	{
		if(GetCurrentPlayersCount() >= ServerData.MaxPlayers)
		{
			DisconnectClient(NewClientActor);
			return;
		}
		FString PlayerName = FString("Player ");
		const uint8 Index = LobbyInfo.PlayerList.Num();
		PlayerName.Append(FString::FromInt(Index));
		LobbyInfo.PlayerList.Add(PlayerName);

		if(AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(NewClientActor))
		{
			Client->SetPlayerIndex(Index);
			Client->SetPlayerName(PlayerName);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Connected Client Valid"))
		OnLobbyUpdated.Broadcast(LobbyInfo);
		UpdateClientLobbyInfo();

		UpdateServerData(ServerData);
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
	UpdateServerData(ServerData);
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

	if(ServerId != -1)
	{
		//REMOVE SERVER ENTRY FROM DATA TABLE
		TSharedRef<IHttpRequest> Request = Http->CreateRequest();
		
		Request->SetURL("https://localhost:44364/api/host/" + FString::FromInt(ServerId));
		Request->SetVerb("DELETE");
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		
		Request->ProcessRequest();
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

void AZombieBeaconHostObject::StartServer(const FString& MapUrl)
{
	for(AOnlineBeaconClient* ClientBeacon: ClientActors)
	{
		if(AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(ClientBeacon))
		{
			Client->Client_FullConnect();
		}
	}

	ShutdownServer();

	if(UWorld* World = GetWorld())
	{
		World->ServerTravel(MapUrl + "?listen");
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

void AZombieBeaconHostObject::SendChatToLobby(const FText& ChatMessage)
{
	OnChatReceived.Broadcast(ChatMessage);
	UE_LOG(LogTemp, Warning, TEXT("Replicating Chat Message Across Clients"))
	for(AOnlineBeaconClient* ClientBeacon: ClientActors)
	{
		if(AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(ClientBeacon))
		{
			Client->Client_OnChatMessageReceived(ChatMessage);
		}
	}
}
