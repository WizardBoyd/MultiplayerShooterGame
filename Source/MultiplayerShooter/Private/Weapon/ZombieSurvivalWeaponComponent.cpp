// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZombieSurvivalWeaponComponent.h"

#include "Camera/CameraComponent.h"
#include "Character/CharacterBase.h"
#include "Character/ZombieBase.h"
#include "Character/ZombieWaveSurvivalCharacter.h"


// Sets default values for this component's properties
UZombieSurvivalWeaponComponent::UZombieSurvivalWeaponComponent()
{
	OwningCharacter = nullptr;
}


// Called when the game starts
void UZombieSurvivalWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UZombieSurvivalWeaponComponent::Fire()
{
	//Super::Fire();
	UE_LOG(LogTemp, Warning, TEXT("FIRING WEAPON"))
	if(OwningCharacter)
	{
		FVector Start = OwningCharacter->GetFirstPeronCameraComponent()->GetComponentLocation();
		FVector Rot = OwningCharacter->GetFirstPeronCameraComponent()->GetComponentRotation().Vector();
		FVector End = Start + Rot * WeaponMaxRange;

		FHitResult HitResult;
		FCollisionObjectQueryParams CollisionQuery;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(OwningCharacter);
		if(GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, CollisionQuery, CollisionParams))
		{
			//TODO: change this for a IHitable interface calling zombie is bad practice
			if(AZombieBase* Zombie = Cast<AZombieBase>(HitResult.GetActor()))
			{
				UE_LOG(LogTemp, Warning, TEXT("ZOMBIE HIT %s"), *Zombie->GetName())
				Zombie->Hit(OwningCharacter);
			}
		}
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 3.0f);
	}
}

void UZombieSurvivalWeaponComponent::AttachWeapon(ACharacterBase* TargetCharacter)
{
	Super::AttachWeapon(TargetCharacter);
	if(AZombieWaveSurvivalCharacter* Temp = Cast<AZombieWaveSurvivalCharacter>(TargetCharacter))
	{
		OwningCharacter = Temp;
	}
}

