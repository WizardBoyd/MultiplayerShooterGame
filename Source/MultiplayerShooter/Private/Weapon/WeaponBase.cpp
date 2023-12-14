// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"

#include "EnhancedInputSubsystems.h"
#include "Character/ZombieWaveSurvivalCharacter.h"
#include "Components/SkeletalMeshComponent.h"

AWeaponBase::AWeaponBase()
{
	BaseDamage = 100;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	RootComponent = WeaponMesh;

	SetReplicates(true);
}
void AWeaponBase::AttachWeapon(AZombieWaveSurvivalCharacter* Character)
{
	if(Character)
	{
		OwningCharacter = Character;
		if(Character->IsLocallyControlled())
		{
			if (APlayerController* PlayerController = Cast<APlayerController>(OwningCharacter->Controller))
			{
				UE_LOG(LogTemp, Warning, TEXT("ATTACHING WEAPON TO: %s"), *OwningCharacter->GetName());
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
				{
					Subsystem->AddMappingContext(WeaponMappingContext, 0);
				}
			}
		}
	}
}

void AWeaponBase::RemoveWeapon()
{
	if(OwningCharacter)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(OwningCharacter->Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(WeaponMappingContext);
			}
		}
	}
}


