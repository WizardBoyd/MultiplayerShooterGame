// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "ZombieWaveSurvivalCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractChanged, const FString&, OnInteractChanged);

class AInteractableBase;
// class UZombieSurvivalWeaponComponent;
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

	UPROPERTY(EditDefaultsOnly)
		float InteractionRange;

	UPROPERTY(EditDefaultsOnly) //Set to replicate
		int32 Points;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;

protected:
	// UPROPERTY(EditDefaultsOnly, Category="Weapons")
	// TSubclassOf<UZombieSurvivalWeaponComponent> SpawningWeapon;

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

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
