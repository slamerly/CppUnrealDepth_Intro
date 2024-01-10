// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE5_INTRODUCTION_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	

// INPUTS
protected:
	UPROPERTY(EditAnywhere, Category = "Input|Movement")
	FName MoveForwardInputName = FName();
	UPROPERTY(EditAnywhere, Category = "Input|Movement")
	FName MoveRightInputName = FName();
	UPROPERTY(EditAnywhere, Category = "Input|Movement")
	FName JumpInputName = FName();
	UPROPERTY(EditAnywhere, Category = "Input|Mouse")
	FName LookUpInputName = FName();
	UPROPERTY(EditAnywhere, Category = "Input|Mouse")
	FName LookRightInputName = FName();
	UPROPERTY(EditAnywhere, Category = "Input|Score")
	FName CountScoreInputName = FName();
	

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void SetupCameraComponent();

	void Jump();

	// Score
protected:
	void CountScore();

protected:
	TArray<AActor*> Goals;

public:
	virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* InPawn) override;

	// mouse sensitive
protected:
	UPROPERTY(EditAnywhere, Category = "Input|Mouse")
	float MouseSensitivityX = 1.f;
	UPROPERTY(EditAnywhere, Category = "Input|Mouse")
	float MouseSensitivityY = 1.f;

public:
	virtual void AddPitchInput(float Val) override;
	virtual void AddYawInput(float Val) override;

// CHARACTER
protected:
	class AMyCharacter* Character = nullptr;

	// Gravity gun
	class UGravityGunController* GravityGunController = nullptr;

public:
	virtual void BeginPlay() override;
	void LateBeginPlay();
};
