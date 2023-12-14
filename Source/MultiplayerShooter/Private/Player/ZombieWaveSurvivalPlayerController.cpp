// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZombieWaveSurvivalPlayerController.h"
#include "Blueprint/UserWidget.h"

AZombieWaveSurvivalPlayerController::AZombieWaveSurvivalPlayerController()
{
	PlayerHud = nullptr;
	static ConstructorHelpers::FClassFinder<UUserWidget> HUDClassFinder(TEXT("/Game/Blueprints/HUD/Player/WBP_HUD.WBP_HUD_C"));
	PlayerHUDClass = HUDClassFinder.Class;
}

void AZombieWaveSurvivalPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	if(HasAuthority())
	{
		SetShowMouseCursor(false);
		const FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		PlayerHud = CreateWidget(this, PlayerHUDClass, FName("PlayerHud"));
		FTimerHandle Delayhandle;
		GetWorldTimerManager().SetTimer(Delayhandle, this, &ThisClass::AddHudToView,0.2f, false);
	}
}

void AZombieWaveSurvivalPlayerController::AddHudToView()
{
	if(PlayerHud && IsLocalController())
	{
		PlayerHud->AddToViewport();
	}
}


