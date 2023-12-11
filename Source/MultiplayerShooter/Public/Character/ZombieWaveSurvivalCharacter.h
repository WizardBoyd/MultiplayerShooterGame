// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "ZombieWaveSurvivalCharacter.generated.h"

class AInteractableBase;
class AWeaponBase;
class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractChanged, const FString&, OnInteractChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPointsChanged, int32, OnPointsChanged);

UCLASS()
class MULTIPLAYERSHOOTER_API AZombieWaveSurvivalCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	AZombieWaveSurvivalCharacter();
protected:

	UPROPERTY(BlueprintAssignable)
		FInteractChanged OnInteractChanged;
	
	FTimerHandle TInteractTimerHandle;
	TObjectPtr<AInteractableBase> Interactable;

	UPROPERTY(BlueprintAssignable)
	FPointsChanged OnPointsChanged;

	UPROPERTY(EditDefaultsOnly)
		float InteractionRange;

	UPROPERTY(EditDefaultsOnly) //Set to replicate
		int32 Points;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;

protected:

	UPROPERTY(EditAnywhere, Category = "Game Settings")
	TSubclassOf<AWeaponBase> StartingWeaponClass;
	
	TObjectPtr<AWeaponBase> CurrentWeapon;
	int32 WeaponIndex;
	TArray<TObjectPtr<AWeaponBase>> WeaponArray;

	//TODO replicate, skip owner
	bool bIsAiming;

protected:
	void Interact();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Interact(AInteractableBase* InteractingObject);

	void Server_Interact_Implementation(AInteractableBase* InteractingObject);
	bool Server_Interact_Validate(AInteractableBase* InteractingObject);
	
	void SetInteractableObject();


public:
	
	void IncrementPoints(uint16 Value);
	bool DecrementPoints(uint16 Value);
	UFUNCTION(BlueprintCallable)
	int32 GetPoints();

	UFUNCTION(BlueprintCallable)
	bool GetIsAiming();

	UFUNCTION(BlueprintCallable)
	void SetIsAiming(bool isAiming);

	void M_PlayAnimation(UAnimMontage* AnimationMontage);

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
