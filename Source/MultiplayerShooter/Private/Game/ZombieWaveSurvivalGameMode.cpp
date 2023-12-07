// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZombieWaveSurvivalGameMode.h"
#include "Game/ZombieWaveSurvivalPlayerSpawnPoint.h"
#include "Character/CharacterBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AZombieWaveSurvivalGameMode::AZombieWaveSurvivalGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFiner(TEXT("/Game/Blueprints/Player/BP_CharacterBase.BP_CharacterBase_C"));
	DefaultPawnClass = PlayerPawnClassFiner.Class;
	bHasloadedSpawnPoints = false;
	
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
			if(APawn* Pawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, Spawnlocation, FRotator::ZeroRotator))
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
	if(TempActors.Num())
	{
		for(AActor* Actor : TempActors)
		{
			if(AZombieWaveSurvivalPlayerSpawnPoint* SpawnPoint = Cast<AZombieWaveSurvivalPlayerSpawnPoint>(Actor))
			{
				PlayerSpawnPoints.Add(SpawnPoint);
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Spawn point Count: %d"), PlayerSpawnPoints.Num())
	bHasloadedSpawnPoints = true;
}

