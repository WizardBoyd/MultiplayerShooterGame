// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "MultiplayerShooter/Input/InputConfigData.h"


ABlasterCharacter::ABlasterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); //Create the Camera Boom UObject
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f; //Could be adjustable
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}


void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlasterCharacter::Move(const FInputActionValue& Value)
{
	if(Controller == nullptr) return;
	const FVector2D MoveValue = Value.Get<FVector2D>();
	const FRotator MovementRotation(0,Controller->GetControlRotation().Yaw, 0);

	if(MoveValue.Y != 0.f)
	{
		const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(Direction, MoveValue.Y);
	}
	if(MoveValue.X != 0.f)
	{
		const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(Direction, MoveValue.X);
	}
}

void ABlasterCharacter::Look(const FInputActionValue& Value)
{
	if(Controller == nullptr) return;
	const FVector2D LookValue = Value.Get<FVector2D>();
	if(LookValue.X != 0.f)
	{
		AddControllerYawInput(LookValue.X);
	}
	if(LookValue.Y != 0.f)
	{
		AddControllerPitchInput(LookValue.Y);
	}
}


void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	/*
	 *	I followed this tutorial for setting up input mapping to C++ character controller
	 *	https://nightails.com/2022/10/16/unreal-engine-enhanced-input-system-in-c/
	 */
	
	//Get the player controller
	APlayerController* PC = Cast<APlayerController>(GetController());

	//Get the local player subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	//Clear out existing mapping and add our mapping
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping,0);

	//Get the enhanced Input Component
	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	//bind actions
	PEI->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this, &ThisClass::Move);
	PEI->BindAction(InputActions->InputLook, ETriggerEvent::Triggered, this, &ThisClass::Look);
}

