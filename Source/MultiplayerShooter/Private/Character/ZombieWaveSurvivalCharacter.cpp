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
	InteractionRange = 200.0f;
	Points = 500;
}

void AZombieWaveSurvivalCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interact Called"))
	//Perform Server RPC if client
	if(Interactable)
	{
		if(HasAuthority())
			Interactable->Use(this);
		else
			Server_Interact(Interactable);
	}
}

void AZombieWaveSurvivalCharacter::Server_Interact_Implementation(AInteractableBase* InteractingObject)
{
	float Distance = GetDistanceTo(InteractingObject);
	if(Distance < InteractionRange + 30.0f)
	{
		InteractingObject->Use(this);
	}
}

bool AZombieWaveSurvivalCharacter::Server_Interact_Validate(AInteractableBase* InteractingObject)
{
	return true;
}

void AZombieWaveSurvivalCharacter::SetInteractableObject()
{
	FVector Start = GetFirstPeronCameraComponent()->GetComponentLocation();
	FVector Rot = GetFirstPeronCameraComponent()->GetComponentRotation().Vector();
	FVector End = Start + Rot * InteractionRange;

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

void AZombieWaveSurvivalCharacter::IncrementPoints(uint16 Value)
{
	Points += Value;
	UE_LOG(LogTemp, Warning, TEXT("Increased Points - Current Points: %d"), Points);
}

bool AZombieWaveSurvivalCharacter::DecrementPoints(uint16 Value)
{
	if((Points - Value) < 0)
	{
		return false;
	}else
	{
		Points -= Value;
		UE_LOG(LogTemp, Warning, TEXT("Decreased Points - Current Points: %d"), Points);
		return true;
	}
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

