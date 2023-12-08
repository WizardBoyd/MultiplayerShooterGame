// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "Barricade.generated.h"

class UStaticMeshComponent;
class USkeletalMeshComponent;
class UAnimationAsset;
UCLASS()
class MULTIPLAYERSHOOTER_API ABarricade : public AInteractableBase
{
	GENERATED_BODY()

public:
	ABarricade();

protected:

	UPROPERTY(EditAnywhere, Category = "Zombie Wave Survival Settings")
	TObjectPtr<USkeletalMeshComponent> MeshComp;
	
	UPROPERTY(EditAnywhere, Category = "Zombie Wave Survival Settings")
	TObjectPtr<UStaticMeshComponent> CollisionMesh;

	UPROPERTY(EditAnywhere, Category = "Zombie Wave Survival Settings")
	TObjectPtr<UAnimationAsset> OpenAnimation;

	UPROPERTY(EditAnywhere, Category = "Zombie Wave Survival Settings")
	uint16 Cost;
	
	UPROPERTY(EditAnywhere, Category = "Zombie Wave Survival Settings")
    	uint8 AccessZone;

	UPROPERTY(ReplicatedUsing = OnRep_BarricadeUsed)
		bool bIsUsed;

	UFUNCTION()
		void OnRep_BarricadeUsed();

protected:

	virtual void Use(AZombieWaveSurvivalCharacter* Player) override;
	virtual void BeginPlay() override;

public:
	uint8 GetAccessZone();
};
