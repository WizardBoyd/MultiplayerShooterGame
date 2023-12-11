// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZombieWaveSurvivalCharacter.h"

#include "EnhancedInputComponent.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Useables/InteractableBase.h"
#include "Weapon/WeaponBase.h"


// Sets default values
AZombieWaveSurvivalCharacter::AZombieWaveSurvivalCharacter()
{
	Interactable = nullptr;
	InteractionRange = 200.0f;
	Points = 500;

	//Setting Weapon Details
	WeaponIndex = 0;
	bIsAiming = false;
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
	OnPointsChanged.Broadcast(Points);
	// UE_LOG(LogTemp, Warning, TEXT("Increased Points - Current Points: %d"), Points);
}

bool AZombieWaveSurvivalCharacter::DecrementPoints(uint16 Value)
{
	if((Points - Value) < 0)
	{
		return false;
	}else
	{
		Points -= Value;
		OnPointsChanged.Broadcast(Points);
		//UE_LOG(LogTemp, Warning, TEXT("Decreased Points - Current Points: %d"), Points);
		return true;
	}
}

int32 AZombieWaveSurvivalCharacter::GetPoints()
{
	return Points;
}

bool AZombieWaveSurvivalCharacter::GetIsAiming()
{
	return bIsAiming;
}

void AZombieWaveSurvivalCharacter::SetIsAiming(bool isAiming)
{
	bIsAiming = isAiming;
}

void AZombieWaveSurvivalCharacter::M_PlayAnimation(UAnimMontage* AnimationMontage)
{
	UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
	if(AnimInstance && AnimationMontage)
	{
		AnimInstance->Montage_Play(AnimationMontage);
	}
}

void AZombieWaveSurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Spawn Weapon Using StartingWeaponClass
	if((CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(StartingWeaponClass)))
	{
		//Attach Spawned Weapon To Socket
		CurrentWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("s_weaponSocket"));
		CurrentWeapon->AttachWeapon(this);
		WeaponArray.Add(CurrentWeapon);
	}
	
	if(UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(TInteractTimerHandle, this, &ThisClass::SetInteractableObject, 0.5f, true);
	}
	
}

void AZombieWaveSurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//TODO: Setup Spawning Weapon Controls to the current controls
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Interaction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ThisClass::Interact);
	}
}


