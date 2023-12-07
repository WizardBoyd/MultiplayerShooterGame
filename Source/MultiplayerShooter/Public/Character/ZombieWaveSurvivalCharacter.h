// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "ZombieWaveSurvivalCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractChanged, const FString&, OnInteractChanged);

class AInteractableBase;
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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;

protected:
	void Interact();
	void SetInteractableObject();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
