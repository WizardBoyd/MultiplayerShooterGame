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
#include "Useables/Barricade.h"


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
		int randomIndex = FMath::RandRange(0, ActiveZombieSpawnPoints.Num() - 1);
		if(AZombieWaveSurvivalZombieSpawnPoint * SpawnPoint = ActiveZombieSpawnPoints[randomIndex])
		{
			FVector location = SpawnPoint->GetActorLocation();
			FRotator rotation = SpawnPoint->GetActorRotation();
			if(AZombieBase* Zombie = GetWorld()->SpawnActor<AZombieBase>(ZombieClass, location, rotation))
			{
				//UE_LOG(LogTemp, Warning, TEXT("SPAWNED Zombie"))
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
			
			if(ABarricade* LinkedBarricade = SpawnPoint->GetLinkedBarricade())
			{
				UE_LOG(LogTemp, Warning, TEXT("Zone Number: %d"), LinkedBarricade->GetAccessZone())
				SpawnPoint->SetZone(LinkedBarricade->GetAccessZone());
				ZombieSpawnPoints.Add(SpawnPoint);
			}else
			{
				ActiveZombieSpawnPoints.Add(SpawnPoint);
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Spawn point Count Zombies: %d"), ZombieSpawnPoints.Num())

	if(UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(TZombieSpawnHandle, this, &ThisClass::SpawnZombie, 2.0f, true); //This is where I may change spawn rate
	}
	
}

void AZombieWaveSurvivalGameMode::NewZoneActive(uint8 ZoneNumber)
{
	UE_LOG(LogTemp, Warning, TEXT("Setting Active Zone %d"), ZoneNumber);

	int Control = 0;
	
	for(int16 x = ZombieSpawnPoints.Num() - 1; x >= 0; --x)
	{
		AZombieWaveSurvivalZombieSpawnPoint* SpawnPoint = ZombieSpawnPoints[x];
		
		UE_LOG(LogTemp, Warning, TEXT("LoopCount %d"), Control);
		++Control;
		if(SpawnPoint && ZoneNumber == SpawnPoint->GetZone() && !SpawnPoint->IsActive())
		{
			ActiveZombieSpawnPoints.Add(SpawnPoint);
			SpawnPoint->Activate();
			ZombieSpawnPoints.RemoveAt(x);
			//Remove spawn point from the ZombieSpawnPoints Array
		}
	}
}

