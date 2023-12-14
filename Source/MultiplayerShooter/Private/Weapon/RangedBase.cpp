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
void ARangedBase::PerformLineTrace(FVector StartLocation, FVector StartRotation, TArray<FHitResult>& Results)
{
	FVector End = StartLocation + StartRotation * InteractionRange;
	
	FCollisionResponseParams CollisionResponse;
	FCollisionQueryParams CollisionParams;
	TArray<AActor*> IgnoreActors = TArray<AActor*>({this});
	if(GetOwner())
		IgnoreActors.Add(GetOwner());
	CollisionParams.AddIgnoredActors(IgnoreActors);
	GetWorld()->LineTraceMultiByChannel(OUT Results, StartLocation, End, ECollisionChannel::ECC_GameTraceChannel1,
			CollisionParams,CollisionResponse);
	DrawDebugLine(GetWorld(), StartLocation, End, FColor::Red, false, 2.0f, 0, 3.0f);
}

void ARangedBase::Fire()
{
	if(GetWorld()->IsNetMode(NM_ListenServer) || GetWorld()->IsNetMode(NM_DedicatedServer) || GetWorld()->IsNetMode(NM_Standalone))
	{
		UE_LOG(LogTemp,Warning,TEXT("FIRING FROM A HOST"));
		if(OwningCharacter)
		{
			if(FireAnimation)
				WeaponMesh->PlayAnimation(FireAnimation, false);
			OwningCharacter->M_PlayAnimation(FPSFireArmsMontage);
		}
		TArray<FHitResult> Results;
		PerformLineTrace(Results);
		if(Results.Num() > 0)
		{
			for(FHitResult& Result: Results)
			{
				if(AZombieBase* Zombie = Cast<AZombieBase>(Result.GetActor()))
				{
					Zombie->Hit(OwningCharacter);
				}
			}
		}
		
	}else if(GetWorld()->IsNetMode(NM_Client))
	{
		UE_LOG(LogTemp,Warning,TEXT("FIRING FROM A CLIENT"));
		if(OwningCharacter)
		{
			if(FireAnimation)
				WeaponMesh->PlayAnimation(FireAnimation, false);
			OwningCharacter->M_PlayAnimation(FPSFireArmsMontage);
		}
		Server_Fire(WeaponMesh->GetSocketLocation(FName("muzzleSocket")),WeaponMesh->GetSocketQuaternion(FName("muzzleSocket")).Vector());
	}
}
void ARangedBase::Server_Fire_Implementation(FVector MuzzleLocation, FVector MuzzleRotation)
{
	//UE_LOG(LogTemp, Warning, TEXT("SERVER FIRE FUNCTION"));
	if(OwningCharacter)
	{
		if(FireAnimation)
		{
			WeaponMesh->PlayAnimation(FireAnimation, false);
		}
		OwningCharacter->M_PlayAnimation(FPSFireArmsMontage);
	}
	TArray<FHitResult> Results;
	PerformLineTrace(MuzzleLocation,MuzzleRotation,Results);
	if(Results.Num() > 0 )
	{
		for(FHitResult& Result: Results)
		{
			if(AZombieBase* Zombie = Cast<AZombieBase>(Result.GetActor()))
			{
				if(AZombieWaveSurvivalCharacter* Player = Cast<AZombieWaveSurvivalCharacter>(GetOwner()))
					Zombie->Hit(Player);
			}
		}
	}
}
bool ARangedBase::Server_Fire_Validate(FVector MuzzleLocation, FVector MuzzleRotation)
{
	return true;
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
	if(Character->IsLocallyControlled())
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(OwningCharacter->InputComponent))
		{
			//Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ThisClass::Fire);

			//Scope
			EnhancedInputComponent->BindAction(ScopeAction, ETriggerEvent::Started, this, &ThisClass::Scope);
			EnhancedInputComponent->BindAction(ScopeAction, ETriggerEvent::Completed, this, &ThisClass::Scope);
		
		}
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



