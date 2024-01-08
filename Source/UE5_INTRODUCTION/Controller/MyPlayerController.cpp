// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/MyCharacter.h"
#include "Controller/MyPlayerController.h"
#include "Controller/GravityGunController.h"

void AMyPlayerController::MoveForward(float Value)
{
	//UE_LOG(LogTemp, Log, TEXT("Move Forward %f"), Value);

	if (Character && Value != 0.f)
		Character->AddMovementInput(Character->GetActorForwardVector(), Value);
}

void AMyPlayerController::MoveRight(float Value)
{
	//UE_LOG(LogTemp, Log, TEXT("Move Right %f"), Value);

	if (Character && Value != 0.f)
		Character->AddMovementInput(Character->GetActorRightVector(), Value);
}

void AMyPlayerController::SetupCameraComponent()
{
	InputComponent->BindAxis(LookUpInputName, Character, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis(LookRightInputName, Character, &APawn::AddControllerYawInput);
}

void AMyPlayerController::Jump()
{
	if (Character)
		Character->Jump();
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind movement inputs
	InputComponent->BindAxis(MoveForwardInputName, this, &AMyPlayerController::MoveForward);
	InputComponent->BindAxis(MoveRightInputName, this, &AMyPlayerController::MoveRight);

	InputComponent->BindAction(JumpInputName, EInputEvent::IE_Pressed, this, &AMyPlayerController::Jump);
}

void AMyPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	Character = Cast<AMyCharacter>(InPawn);

	// bind camera
	SetupCameraComponent();

	//bind jump
	Jump();


}

void AMyPlayerController::AddPitchInput(float Val)
{
	float Multiplier = Val * MouseSensitivityY;
	Super::AddPitchInput(Multiplier);
}

void AMyPlayerController::AddYawInput(float Val)
{
	float Multiplier = Val * MouseSensitivityX;
	Super::AddYawInput(Multiplier);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->OnWorldBeginPlay.AddUObject(this, &AMyPlayerController::LateBeginPlay);
}

void AMyPlayerController::LateBeginPlay()
{
	GravityGunController = GetComponentByClass<UGravityGunController>();

	if (GravityGunController)
		GravityGunController->SetupInputComponentGravityGun(Character, InputComponent);
}
