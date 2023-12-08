// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"

class AZombieWaveSurvivalCharacter;
UCLASS()
class MULTIPLAYERSHOOTER_API AInteractableBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableBase();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Zombie Wave Survival Settings")
	FString UIMessage;

	UPROPERTY(EditAnywhere, Category = "Zombie Wave Survival Settings")
	FString ObjectName;

protected:
	virtual void BeginPlay() override;

public:
	FString GetUIMessage();

	virtual void Use(AZombieWaveSurvivalCharacter* Player);
};
