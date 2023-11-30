// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UCustomGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:

	UCustomGameInstanceBase();

protected:

	UFUNCTION(BlueprintCallable)
	void TravelToMap(const FString& MapUrl);
};
