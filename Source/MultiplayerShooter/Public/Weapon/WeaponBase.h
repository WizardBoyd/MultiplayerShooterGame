// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class AZombieWaveSurvivalCharacter;
class UInputMappingContext;

UCLASS()
class MULTIPLAYERSHOOTER_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

protected:

	UPROPERTY(EditAnywhere, Category = "Zombie Wave Survival Settings")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	TObjectPtr<AZombieWaveSurvivalCharacter> OwningCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> WeaponMappingContext;

	UPROPERTY(EditAnywhere, Category = "Weapon Configuration")
	int32 BaseDamage;
	
public:
	virtual void AttachWeapon(AZombieWaveSurvivalCharacter* Character);
	virtual void RemoveWeapon();
	
};
