// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SpawnerController.h"
#include "Component/SpawnerPickUpComponent.h"
#include "Component/GravityGunComponent.h"
#include "Character/MyCharacter.h"

// Sets default values for this component's properties
USpawnerController::USpawnerController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USpawnerController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USpawnerController::SetupInputComponentSpawnerPickUp(AMyCharacter* InCharacter, TObjectPtr<class UInputComponent> InputComponent)
{
	Character = InCharacter;
	SpawnerPickUpComponent = Character->GetComponentByClass<USpawnerPickUpComponent>();

	class UGravityGunComponent* GravityGunComponent = Character->GetComponentByClass<UGravityGunComponent>();
	if (GravityGunComponent)
		SpawnerPickUpComponent->SetGravityGunComponent(GravityGunComponent);


	SpawnerPickUpComponent->SetCharacter(Character);

	InputComponent->BindAction(SpawnPickUpNormalInputName, EInputEvent::IE_Pressed, this, &USpawnerController::OnSpawnPickUpNormalInputPressed);
	InputComponent->BindAction(SpawnPickUpDATakeInputName, EInputEvent::IE_Pressed, this, &USpawnerController::OnSpawnPickUpDATakeInputPressed);
	InputComponent->BindAction(SpawnPickUpDAThrowInputName, EInputEvent::IE_Released, this, &USpawnerController::OnSpawnPickUpDAThrowInputPressed);
	InputComponent->BindAction(CountPickUpInputName, EInputEvent::IE_Pressed, this, &USpawnerController::OnCountPickUpInputPressed);
	InputComponent->BindAction(DestroyPickUpInputName, EInputEvent::IE_Pressed, this, &USpawnerController::OnDestroyPickUpInputPressed);

}

void USpawnerController::OnSpawnPickUpNormalInputPressed()
{
	if (SpawnerPickUpComponent)
		SpawnerPickUpComponent->OnSpawnPickUpNormalInputPressed();
}

void USpawnerController::OnSpawnPickUpDATakeInputPressed()
{
	if (SpawnerPickUpComponent)
		SpawnerPickUpComponent->OnSpawnPickUpDATakeInputPressed();
}

void USpawnerController::OnSpawnPickUpDAThrowInputPressed()
{
	if (SpawnerPickUpComponent)
		SpawnerPickUpComponent->OnSpawnPickUpDAThrowInputPressed();
}

void USpawnerController::OnCountPickUpInputPressed()
{
	if (SpawnerPickUpComponent)
		SpawnerPickUpComponent->OnCountPickUpInputPressed();
}

void USpawnerController::OnDestroyPickUpInputPressed()
{
	if (SpawnerPickUpComponent)
		SpawnerPickUpComponent->OnDestroyPickUpInputPressed();
}

