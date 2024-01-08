// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/GravityGunController.h"
#include "Component/GravityGunComponent.h"
#include "Character/MyCharacter.h"

// Sets default values for this component's properties
UGravityGunController::UGravityGunController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UGravityGunController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGravityGunController::SetupInputComponentGravityGun(AMyCharacter* InCharacter, TObjectPtr<class UInputComponent> InputComponent)
{
	Character = InCharacter;
	GravityGunComponent = Character->GetComponentByClass<UGravityGunComponent>();

	GravityGunComponent->SetCharacter(Character);

	InputComponent->BindAction(TakeObjectInputName, EInputEvent::IE_Pressed, this, &UGravityGunController::OnTakeObjectInputPressed);
	InputComponent->BindAction(ThrowObjectInputName, EInputEvent::IE_Pressed, this, &UGravityGunController::OnThrowObjectInputPressed);

}

void UGravityGunController::OnTakeObjectInputPressed()
{
	if (GravityGunComponent)
		GravityGunComponent->OnTakeObjectInputPressed();

}

void UGravityGunController::OnThrowObjectInputPressed()
{
	if (GravityGunComponent)
		GravityGunComponent->OnThrowObjectInputPressed();
}

