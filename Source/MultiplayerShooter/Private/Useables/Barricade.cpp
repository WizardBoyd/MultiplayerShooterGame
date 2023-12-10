// Fill out your copyright notice in the Description page of Project Settings.


#include "Useables/Barricade.h"

#include "Character/ZombieWaveSurvivalCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Game/ZombieWaveSurvivalGameMode.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ABarricade::ABarricade()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComp;
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	Cost = 500;
	AccessZone = 0;
	UIMessage += "Door [Cost: " + FString::FromInt(Cost) + "]";
	bIsUsed = false;
}

void ABarricade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABarricade, bIsUsed);
}

void ABarricade::OnRep_BarricadeUsed()
{
	//Play Animation to move barricade
	//SetActorEnableCollision(false);
	//Check if animation is valid
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(OpenAnimation)
		MeshComp->PlayAnimation(OpenAnimation, false);
	UIMessage = FString();
}

void ABarricade::Use(AZombieWaveSurvivalCharacter* Player)
{
	//Play Animation to move barricade
	if(HasAuthority() && !bIsUsed && Player && Player->DecrementPoints(Cost))
	{
		UE_LOG(LogTemp, Warning, TEXT("In use Function For %s"), *GetName())
		bIsUsed = true;
		OnRep_BarricadeUsed();

		if(AZombieWaveSurvivalGameMode* GM = GetWorld()->GetAuthGameMode<AZombieWaveSurvivalGameMode>())
		{
			GM->NewZoneActive(AccessZone);
		}
	}
}

void ABarricade::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	// UIMessage += FString("[Cost: " + FString::FromInt(Cost) + "]");
}

uint8 ABarricade::GetAccessZone()
{
	return AccessZone;
}

