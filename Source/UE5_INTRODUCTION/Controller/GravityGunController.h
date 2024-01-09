// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityGunController.generated.h"


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_INTRODUCTION_API UGravityGunController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGravityGunController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Inputs
protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	FName TakeObjectInputName = FName();
	UPROPERTY(EditAnywhere, Category = "Input")
	FName ThrowObjectInputName = FName();
	UPROPERTY(EditAnywhere, Category = "Input")
	FName UpSizeRaycastInputName = FName();
	UPROPERTY(EditAnywhere, Category = "Input")
	FName DownSizeRaycastInputName = FName();

protected:
	class AMyCharacter* Character = nullptr;
	class UGravityGunComponent* GravityGunComponent = nullptr;

public:
	void SetupInputComponentGravityGun(class AMyCharacter* InCharacter, TObjectPtr<class UInputComponent> InputComponent);

protected:
	void OnTakeObjectInputPressed();
	void OnThrowObjectInputPressed();
	void OnUpSizeRaycastInputPressed();
	void OnThrowObjectInputReleased();
		
};
