// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CustomGameInstanceBase.h"
#include "Engine/World.h"

UCustomGameInstanceBase::UCustomGameInstanceBase()
{
	
}

void UCustomGameInstanceBase::TravelToMap(const FString& MapUrl)
{
	UWorld* world = GetWorld();
	world->ServerTravel(MapUrl);
}
