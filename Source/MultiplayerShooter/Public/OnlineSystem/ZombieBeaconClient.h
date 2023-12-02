// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "ZombieBeaconClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectSuccessful, bool, bConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisconnected);

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

protected:
	UPROPERTY(BlueprintAssignable)
	FConnectSuccessful OnClientConnected;

	UPROPERTY(BlueprintAssignable)
	FDisconnected OnClientDisconnected;
};
