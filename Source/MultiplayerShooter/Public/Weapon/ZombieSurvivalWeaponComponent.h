// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponComponent.h"
#include "ZombieSurvivalWeaponComponent.generated.h"

class AZombieWaveSurvivalCharacter;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTIPLAYERSHOOTER_API UZombieSurvivalWeaponComponent : public UWeaponComponent
{
	GENERATED_BODY()

public:
	UZombieSurvivalWeaponComponent();

protected:
	UPROPERTY(EditAnywhere, Category="Weapon Settings")
		float WeaponMaxRange = 2000.0f;

	TObjectPtr<AZombieWaveSurvivalCharacter> OwningCharacter;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Fire() override;

	virtual void AttachWeapon(ACharacterBase* TargetCharacter) override;
};
