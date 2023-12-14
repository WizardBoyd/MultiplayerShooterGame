// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZombieWaveSurvivalPlayerController.generated.h"
class UUserWidget;
UCLASS()
class MULTIPLAYERSHOOTER_API AZombieWaveSurvivalPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AZombieWaveSurvivalPlayerController();
protected:
	virtual void BeginPlayingState() override;

	UPROPERTY(EditAnywhere, Category = "Game Settings")
	TSubclassOf<UUserWidget> PlayerHUDClass;

	TObjectPtr<UUserWidget> PlayerHud;

private:
	void AddHudToView();

};
