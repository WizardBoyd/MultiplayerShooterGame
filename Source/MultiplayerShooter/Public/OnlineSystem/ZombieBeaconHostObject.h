// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconHostObject.h"
#include "http.h"
#include "Game/CustomGameInstanceBase.h"
#include "ZombieBeaconHostObject.generated.h"

USTRUCT(BlueprintType)
struct FZombieLobbyInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class UTexture2D> MapImage;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> PlayerList;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHostLobbyUpdated, FZombieLobbyInfo, FOnLobbyUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHostChatReceived, const FText&, FOnHostChatReceived);

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API AZombieBeaconHostObject : public AOnlineBeaconHostObject
{
	GENERATED_BODY()

public:
	AZombieBeaconHostObject();

protected:

	FHttpModule* Http;
	int ServerId;
	void OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);
	
protected:

	FServerData ServerData;
	FZombieLobbyInfo LobbyInfo;

	UFUNCTION(BlueprintCallable)
	void SetServerData(const FServerData& NewServerData);

	UFUNCTION(BlueprintCallable)
	void UpdateServerData(const FServerData& NewServerData);

	UFUNCTION(BlueprintCallable)
	int GetCurrentPlayersCount();

	UPROPERTY(BlueprintAssignable)
	FHostLobbyUpdated OnLobbyUpdated;

	UPROPERTY(BlueprintAssignable)
	FHostChatReceived OnChatReceived;

	FTimerHandle TInitialLobbyHandle;

	UFUNCTION(BlueprintCallable)
	void UpdateLobbyInfo(FZombieLobbyInfo NewLobbyInfo);


	void UpdateClientLobbyInfo();

	void InitialLobbyHandling();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection) override;
	virtual void NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor) override;

	UFUNCTION(BlueprintCallable)
	void ShutdownServer();
	
	void DisconnectAllClients();

	virtual void DisconnectClient(AOnlineBeaconClient* ClientActor) override;

	UFUNCTION(BlueprintCallable)
	void StartServer(const FString& MapUrl);

private:

	void ReAssignPlayerIDs();

public:
	UFUNCTION(BlueprintCallable)
	void SendChatToLobby(const FText& ChatMessage);
};
