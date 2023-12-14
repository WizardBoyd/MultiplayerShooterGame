// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "ZombieWaveSurvivalCharacter.generated.h"

class AInteractableBase;
class AWeaponBase;
class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractChanged, const FString&, OnInteractChanged);

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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;

protected:

	UPROPERTY(EditAnywhere, Category = "Game Settings")
	TSubclassOf<AWeaponBase> StartingWeaponClass;


	UPROPERTY(ReplicatedUsing = OnRep_AttachWeapon)
	TObjectPtr<AWeaponBase> CurrentWeapon;

	UFUNCTION()
	void OnRep_AttachWeapon();
	
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

	UFUNCTION(BlueprintCallable)
	bool GetIsAiming();

	UFUNCTION(BlueprintCallable)
	void SetIsAiming(bool isAiming);

	void M_PlayAnimation(UAnimMontage* AnimationMontage);

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
