// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZombieWaveSurvivalGameMode.h"
#include "Game/ZombieWaveSurvivalPlayerSpawnPoint.h"
#include "Character/CharacterBase.h"
#include "Character/ZombieBase.h"
#include "Game/ZombieWaveSurvivalZombieSpawnPoint.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Character/ZombieWaveSurvivalCharacter.h"
#include "Game/ZombieSurvivalGameState.h"


// Sets default values
AZombieWaveSurvivalGameMode::AZombieWaveSurvivalGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFiner(TEXT("/Game/Blueprints/Player/BP_ZombieWaveSurvivalCharacter.BP_ZombieWaveSurvivalCharacter_C"));
	DefaultPawnClass = PlayerPawnClassFiner.Class;
	bHasloadedSpawnPoints = false;
	ZombiesRemaining = 0;
	
}

void AZombieWaveSurvivalGameMode::SpawnZombie()
{
	if(ZombiesRemaining > 0)
	{
		int randomIndex = FMath::RandRange(0, ZombieSpawnPoints.Num() - 1);
		if(AZombieWaveSurvivalZombieSpawnPoint * SpawnPoint = ZombieSpawnPoints[randomIndex])
		{
			FVector location = SpawnPoint->GetActorLocation();
			FRotator rotation = SpawnPoint->GetActorRotation();
			if(AZombieBase* Zombie = GetWorld()->SpawnActor<AZombieBase>(ZombieClass, location, rotation))
			{
				UE_LOG(LogTemp, Warning, TEXT("SPAWNED Zombie"))
				--ZombiesRemaining;
			}
		}
	}else
	{
		if(UWorld* World = GetWorld())
		{
			World->GetTimerManager().PauseTimer(TZombieSpawnHandle);
		}
	}
}

void AZombieWaveSurvivalGameMode::CalculateZombieCount()
{
	if(ZombieGameState)
	{
		uint16 RoundNumber = ZombieGameState->GetRoundNumber();
		//do calculation here
		ZombiesRemaining = 5;
	}
}

void AZombieWaveSurvivalGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(bHasloadedSpawnPoints == false)
		SetSpawnPoints();

	for (AZombieWaveSurvivalPlayerSpawnPoint* SpawnPoint : PlayerSpawnPoints)
	{
		if(!SpawnPoint->IsUsed())
		{
			FVector Spawnlocation = SpawnPoint->GetActorLocation();
			if(APawn* Pawn = GetWorld()->SpawnActor<APawn>(PlayerClass, Spawnlocation, FRotator::ZeroRotator))
			{
				UE_LOG(LogTemp, Warning, TEXT("SPAWNED PAWN TO POSSESS"))
				NewPlayer->Possess(Pawn);
				SpawnPoint->SetUsed(true);
				return;
			}
		}
	}

}

void AZombieWaveSurvivalGameMode::SetSpawnPoints()
{
	TArray<AActor*> TempActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombieWaveSurvivalPlayerSpawnPoint::StaticClass(), TempActors);
	for(AActor* Actor : TempActors)
	{
		if(AZombieWaveSurvivalPlayerSpawnPoint* SpawnPoint = Cast<AZombieWaveSurvivalPlayerSpawnPoint>(Actor))
		{
			PlayerSpawnPoints.Add(SpawnPoint);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Spawn point Count: %d"), PlayerSpawnPoints.Num())
	bHasloadedSpawnPoints = true;
}

void AZombieWaveSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	ZombieGameState = GetGameState<AZombieSurvivalGameState>();
	CalculateZombieCount();
	
	TArray<AActor*> TempActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombieWaveSurvivalZombieSpawnPoint::StaticClass(), TempActors);
	for(AActor* Actor : TempActors)
	{
		if(AZombieWaveSurvivalZombieSpawnPoint* SpawnPoint = Cast<AZombieWaveSurvivalZombieSpawnPoint>(Actor))
		{
			ZombieSpawnPoints.Add(SpawnPoint);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Spawn point Count Zombies: %d"), ZombieSpawnPoints.Num())

	if(UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(TZombieSpawnHandle, this, &ThisClass::SpawnZombie, 2.0f, true); //This is where I may change spawn rate
	}
	
}

