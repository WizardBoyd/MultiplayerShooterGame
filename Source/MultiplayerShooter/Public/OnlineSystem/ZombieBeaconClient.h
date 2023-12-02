// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "MultiplayerShooter/Public/OnlineSystem/ZombieBeaconHostObject.h"
#include "ZombieBeaconClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectSuccessful, bool, bConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisconnected);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLobbyUpdated, FZombieLobbyInfo, FOnLobbyUpdated);

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API AZombieBeaconClient : public AOnlineBeaconClient
{
	GENERATED_BODY()

public:
	AZombieBeaconClient();

protected:
	UFUNCTION(BlueprintCallable)
	bool ConnectToServer(const FString& Address);

	UFUNCTION(BlueprintCallable)
	void LeaveLobby();
	
	virtual void OnFailure() override;
	virtual void OnConnected() override;

public:
	UFUNCTION(Client, Reliable)
	void Client_OnDisconnected();

	virtual void Client_OnDisconnected_Implementation();

	UFUNCTION(Client,Reliable)
	void Client_OnLobbyUpdated(FZombieLobbyInfo LobbyInfo);

	virtual void Client_OnLobbyUpdated_Implementation(FZombieLobbyInfo LobbyInfo);

	void SetPlayerIndex(uint8 Index);
	uint8 GetPlayerIndex();

protected:
	UPROPERTY(BlueprintAssignable)
	FConnectSuccessful OnClientConnected;

	UPROPERTY(BlueprintAssignable)
	FDisconnected OnClientDisconnected;

	UPROPERTY(BlueprintAssignable)
	FLobbyUpdated OnLobbyUpdated;

	uint8 PlayerIndex;
};
