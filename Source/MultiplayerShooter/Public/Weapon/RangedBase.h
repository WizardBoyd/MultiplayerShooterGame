// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "RangedBase.generated.h"

class UAnimationAsset;
class UInputAction;
class UAnimMontage;

UCLASS()
class MULTIPLAYERSHOOTER_API ARangedBase : public AWeaponBase
{
	GENERATED_BODY()

public:
	ARangedBase();

protected:
	UPROPERTY(EditAnywhere, Category = "Weapon Configuration")
	int32 WeaponMaxAmmo;

	UPROPERTY(EditAnywhere, Category = "Weapon Configuration")
	int32 MagazineMaxAmmo;

	UPROPERTY(EditAnywhere, Category = "Weapon Configuration")
	float InteractionRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ScopeAction;

	UPROPERTY(EditAnywhere, Category = "Weapon Animation")
	TObjectPtr<UAnimMontage> FPSFireArmsMontage;
	
	UPROPERTY(EditAnywhere, Category = "Weapon Animation")
	TObjectPtr<UAnimationAsset> FireAnimation;

	UPROPERTY(EditAnywhere, Category = "Weapon Animation")
	TObjectPtr<UAnimationAsset> ReloadAnimation;
	

	int32 CurrentTotalAmmo;
	int32 CurrentMagazineAmmo;

	void PerformLineTrace(TArray<FHitResult>& Results);
	void PerformLineTrace(FVector StartLocation, FVector StartRotation, TArray<FHitResult>& Results);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire(FVector MuzzleLocation, FVector MuzzleRotation);
	virtual void Server_Fire_Implementation(FVector MuzzleLocation, FVector MuzzleRotation);
	bool Server_Fire_Validate(FVector MuzzleLocation, FVector MuzzleRotation);

	virtual void Fire();
	virtual void Reload();
	virtual void Scope();

public:
	//virtual void AttachWeapon(AZombieWaveSurvivalCharacter* Character) override;
	virtual void AttachWeapon(AZombieWaveSurvivalCharacter* Character) override;
	virtual void RemoveWeapon() override;

	/**
	 * @brief Gets the current ammo of the weapon
	 * @return Array of int32 element 1st is magazine ammo, 2nd element is total ammo
	 */
	TArray<int32> GetCurrentAmmo();

	UAnimMontage* GetFireAnimMontage();
};
