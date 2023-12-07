// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "Barricade.generated.h"

class UStaticMeshComponent;
UCLASS()
class MULTIPLAYERSHOOTER_API ABarricade : public AInteractableBase
{
	GENERATED_BODY()

public:
	ABarricade();

protected:
	UPROPERTY(EditAnywhere, Category = "Zombie Wave Survival Settings")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, Category = "Zombie Wave Survival Settings")
	uint16 Cost;

protected:

	virtual void Use(ACharacterBase* Player) override;
	virtual void BeginPlay() override;
};
