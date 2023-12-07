// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "http.h"
#include "CustomGameInstanceBase.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FMapInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapUrl;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapDescription;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* MapImage;
};

USTRUCT(BlueprintType)
struct FServerData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ServerID = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString IPAddress;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ServerName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentPlayers = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxPlayers = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FServersReceived);

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UCustomGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:

	UCustomGameInstanceBase();

	const FString& GetWebApiUrl();

protected:
	FHttpModule* Http;
	const FString WebApiUrl = FString("https://worldatwarz.gear.host/api/host/");
	UPROPERTY(BlueprintAssignable)
	FServersReceived OnServersReceived;

	TArray<FServerData> ServerList;

	UFUNCTION(BlueprintCallable)
	TArray<FServerData>& GetServerList();
	
	void OnServerListRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);

	UFUNCTION(BlueprintCallable)
	void GenerateServerList();

protected:

	UFUNCTION(BlueprintCallable)
	void TravelToMap(const FString& MapUrl);
};
