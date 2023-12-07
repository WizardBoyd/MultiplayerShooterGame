// Fill out your copyright notice in the Description page of Project Settings.


#include "Useables/Barricade.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ABarricade::ABarricade()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeshComp;
	Cost = 500;
	UIMessage += "Door [Cost: " + FString::FromInt(Cost) + "]";
}

void ABarricade::Use(ACharacterBase* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("In use Function For %s"), *GetName())
	SetActorEnableCollision(false);
}

void ABarricade::BeginPlay()
{
	Super::BeginPlay();
	// UIMessage += FString("[Cost: " + FString::FromInt(Cost) + "]");
}

