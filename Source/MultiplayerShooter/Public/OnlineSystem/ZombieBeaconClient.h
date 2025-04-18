// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "MultiplayerShooter/Public/OnlineSystem/ZombieBeaconHostObject.h"
#include "ZombieBeaconClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectSuccessful, bool, bConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisconnected);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLobbyUpdated, FZombieLobbyInfo, FOnLobbyUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChatReceived, const FText&, FOnChatReceived);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFullConnect);

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
	void FullConnectToServer(const FString& JoinAddress);

	UFUNCTION(BlueprintCallable)
	void LeaveLobby();
	
	virtual void OnFailure() override;
	virtual void OnConnected() override;

protected:
	/*
	 *	Chat related Features
	 */
	UFUNCTION(BlueprintCallable)
	void SendChatMessage(FText ChatMessage);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendChatMessage(const FText& ChatMessage);
	void Server_SendChatMessage_Implementation(const FText& ChatMessage);

	/**
	 * @brief 
	 * @param ChatMessage 
	 * @return 
	 */
	bool Server_SendChatMessage_Validate(const FText& ChatMessage);

public:
	UFUNCTION(Client, Reliable)
	void Client_OnDisconnected();

	virtual void Client_OnDisconnected_Implementation();

	UFUNCTION(Client,Reliable)
	void Client_OnLobbyUpdated(FZombieLobbyInfo LobbyInfo);

	UFUNCTION(Client, Reliable)
	void Client_OnChatMessageReceived(const FText& ChatMessage);
	void Client_OnChatMessageReceived_Implementation(const FText& ChatMessage);

	virtual void Client_OnLobbyUpdated_Implementation(FZombieLobbyInfo LobbyInfo);

	void SetPlayerIndex(uint8 Index);
	uint8 GetPlayerIndex();

	void SetPlayerName(const FString& NewPlayerName);
	FString GetPlayerName();

	UFUNCTION(Client, Reliable)
	void Client_FullConnect();

	
	virtual void Client_FullConnect_Implementation();

protected:
	UPROPERTY(BlueprintAssignable)
	FConnectSuccessful OnClientConnected;

	UPROPERTY(BlueprintAssignable)
	FDisconnected OnClientDisconnected;

	UPROPERTY(BlueprintAssignable)
	FLobbyUpdated OnLobbyUpdated;

	UPROPERTY(BlueprintAssignable)
	FChatReceived OnChatReceived;

	UPROPERTY(BlueprintAssignable)
	FFullConnect OnFullConnect;

	uint8 PlayerIndex;
	FString PlayerName;
};
