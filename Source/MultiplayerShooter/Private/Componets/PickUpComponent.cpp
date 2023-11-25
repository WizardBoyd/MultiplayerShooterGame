// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerShooter/Public/Componets/PickUpComponent.h"


// Sets default values for this component's properties
UPickUpComponent::UPickUpComponent()
{
	//setup sphere collision range
	SphereRadius = 32.f;
}


// Called when the game starts
void UPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap);
}

void UPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ACharacterBase* Character = Cast<ACharacterBase>(OtherActor); Character != nullptr)
	{
		//notify that the actor has been picked up
		OnPickUp.Broadcast(Character);

		//Unregister from the overlap event so it no longer triggers
		OnComponentBeginOverlap.RemoveAll(this);
	}
}


