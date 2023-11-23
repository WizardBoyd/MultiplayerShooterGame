// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"


// Sets default values
ALobbyGameMode::ALobbyGameMode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALobbyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//Get the number of players that have joined
	int32 NumberOfPlayers = GameState->PlayerArray.Num();
	if(NumberOfPlayers == 2) //Currently hardcoded for quick testing purposes
	{
		if(UWorld* World = GetWorld())
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Maps/BlasterMap?listen"));
		}
	}
}

