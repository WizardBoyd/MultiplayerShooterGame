// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RangedBase.h"
#include "EnhancedInputComponent.h"
#include "Character/ZombieBase.h"
#include "Animation/AnimInstance.h"
#include "Character/ZombieWaveSurvivalCharacter.h"

ARangedBase::ARangedBase()
{
	InteractionRange = 0.0f;
}
TArray<int32> ARangedBase::GetCurrentAmmo()
{
	return {CurrentMagazineAmmo, CurrentTotalAmmo};
}

UAnimMontage* ARangedBase::GetFireAnimMontage()
{
	return FPSFireArmsMontage;
}

void ARangedBase::PerformLineTrace(TArray<FHitResult>& Results)
{
	FVector Start = WeaponMesh->GetSocketLocation(FName("muzzleSocket"));
	FVector Rot = WeaponMesh->GetSocketQuaternion(FName("muzzleSocket")).Vector();
	FVector End = Start + Rot * InteractionRange;
	
	FCollisionResponseParams CollisionResponse;
	FCollisionQueryParams CollisionParams;
	const TArray<AActor*> IgnoreActors = TArray<AActor*>({OwningCharacter, this});
	CollisionParams.AddIgnoredActors(IgnoreActors);
	GetWorld()->LineTraceMultiByChannel(OUT Results, Start, End, ECollisionChannel::ECC_GameTraceChannel1,
			CollisionParams,CollisionResponse);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 3.0f);
}

void ARangedBase::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("FIRING WEAPON"))
	if(OwningCharacter)
	{
		if(FireAnimation)
		{
			WeaponMesh->PlayAnimation(FireAnimation, false);
		}
		OwningCharacter->M_PlayAnimation(FPSFireArmsMontage);
		TArray<FHitResult> Results;
		PerformLineTrace(Results);
		if(Results.Num() > 0 )
		{
			for(FHitResult& Result: Results)
			{
				if(AZombieBase* Zombie = Cast<AZombieBase>(Result.GetActor()))
				{
					Zombie->Hit(OwningCharacter);
				}
			}
		}
	}
}
void ARangedBase::Reload()
{
}

void ARangedBase::Scope()
{
	if(OwningCharacter)
	{
		const bool IsScoping = OwningCharacter->GetIsAiming();
		OwningCharacter->SetIsAiming(!IsScoping);
	}
}

void ARangedBase::AttachWeapon(AZombieWaveSurvivalCharacter* Character)
{
	Super::AttachWeapon(Character);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(OwningCharacter->InputComponent))
	{
		//Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ThisClass::Fire);

		//Scope
		EnhancedInputComponent->BindAction(ScopeAction, ETriggerEvent::Started, this, &ThisClass::Scope);
		EnhancedInputComponent->BindAction(ScopeAction, ETriggerEvent::Completed, this, &ThisClass::Scope);
		
	}
}

void ARangedBase::RemoveWeapon()
{
	Super::RemoveWeapon();
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(OwningCharacter->InputComponent))
	{
		//Interaction
		EnhancedInputComponent->ClearActionBindings();
	}
}



