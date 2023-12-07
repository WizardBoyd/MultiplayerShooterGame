// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZombieWaveSurvivalCharacter.h"

#include "EnhancedInputComponent.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Useables/InteractableBase.h"


// Sets default values
AZombieWaveSurvivalCharacter::AZombieWaveSurvivalCharacter()
{
	Interactable = nullptr;
}

void AZombieWaveSurvivalCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interact Called"))
	if(Interactable)
	{
		Interactable->Use(this);
	}
}

void AZombieWaveSurvivalCharacter::SetInteractableObject()
{
	FVector Start = GetFirstPeronCameraComponent()->GetComponentLocation();
	FVector Rot = GetFirstPeronCameraComponent()->GetComponentRotation().Vector();
	FVector End = Start + Rot * 500.0f;

	FHitResult HitResult;
	FCollisionObjectQueryParams CollisionQuery;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, CollisionQuery, CollisionParams);
	
	AInteractableBase* Temp = Cast<AInteractableBase>(HitResult.GetActor());
	if(Interactable == nullptr && Temp)
	{
		UE_LOG(LogTemp, Warning, TEXT("IS Now A Valid pointer"));
		Interactable = Temp;
		//Fire Event
		OnInteractChanged.Broadcast(Interactable->GetUIMessage());
		//UE_LOG(LogTemp, Warning, TEXT("HIT ACTOR %s"), *Interactable->GetName());
	}
	else if(Interactable && Temp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("IS Now A NULL pointer"));
		Interactable = nullptr;
		OnInteractChanged.Broadcast(FString());
	}

	// if(Temp)
	// 	Interactable = Temp;
	// else
	// 	Interactable = nullptr;
}

void AZombieWaveSurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(TInteractTimerHandle, this, &ThisClass::SetInteractableObject, 0.5f, true);
	}
	
}

void AZombieWaveSurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Interaction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ThisClass::Interact);
	}
}

