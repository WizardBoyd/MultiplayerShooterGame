// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerShooterGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiplayerShooterGameModeBase();

	virtual void PostLogin(APlayerController* NewPlayer) override;
protected:
	virtual void BeginPlay() override;
};
