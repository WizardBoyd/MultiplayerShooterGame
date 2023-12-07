// Fill out your copyright notice in the Description page of Project Settings.


#include "Useables/InteractableBase.h"
#include "Character/CharacterBase.h"



AInteractableBase::AInteractableBase()
{
	ObjectName = "Default";
	UIMessage = "Press F to buy ";
}

void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
}

FString AInteractableBase::GetUIMessage()
{
	return UIMessage;
}

void AInteractableBase::Use(ACharacterBase* Player)
{
}


