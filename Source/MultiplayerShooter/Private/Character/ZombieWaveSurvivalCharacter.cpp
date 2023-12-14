// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZombieWaveSurvivalCharacter.h"

#include "EnhancedInputComponent.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Useables/InteractableBase.h"
#include "Weapon/WeaponBase.h"


// Sets default values
AZombieWaveSurvivalCharacter::AZombieWaveSurvivalCharacter()
{
	Interactable = nullptr;
	InteractionRange = 200.0f;
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
	//NOTE: does not mean you have authority over the server just means you have authority over this specific instance of the actor

	UE_LOG(LogTemp, Warning, TEXT("BEGIN PLAY WITH: %s") ,*GetName());
	UE_LOG(LogTemp, Warning, TEXT("BEGIN PLAY IN NET MODE: %d") ,GetWorld()->GetNetMode());
	if(HasAuthority())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		UE_LOG(LogTemp, Warning, TEXT("WEAPON OWNER: %s") ,*GetName());
		if((CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(StartingWeaponClass, SpawnParameters)))
		{
			WeaponArray.Add(CurrentWeapon);
			OnRep_AttachWeapon();
		}
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
void AZombieWaveSurvivalCharacter::OnRep_AttachWeapon()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->AttachWeapon(this);
		if(IsLocallyControlled())
		{
			CurrentWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("s_weaponSocket"));
		}else
		{
			
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("s_weaponSocket"));
		}
	}
}

void AZombieWaveSurvivalCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZombieWaveSurvivalCharacter, CurrentWeapon);
}


