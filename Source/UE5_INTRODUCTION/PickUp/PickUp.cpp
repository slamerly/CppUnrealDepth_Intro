// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp/PickUp.h"

// Sets default values
APickUp::APickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUp::StartPickUpDetonationTimer()
{
	float DestructionTime = PickUpStruct.DestructionTimer;

	// Prepare Timer
	FTimerManager& TimerManager = this->GetWorldTimerManager();
	TimerManager.ClearTimer(ProjectileDestructionionTimerHandle);
	TimerManager.SetTimer(ProjectileDestructionionTimerHandle, this, &APickUp::DestroyPickUp, DestructionTime, false);
}

void APickUp::DestroyPickUp()
{
	ClearTimer();

	OnPickUpDestroy.Broadcast();

	Destroy();
}

EPickUpType APickUp::GetPickUpType()
{
	return PickUpStruct.PickUpType;
}

void APickUp::ClearTimer()
{
	// Clear Timer
	FTimerManager& TimerManager = this->GetWorldTimerManager();
	TimerManager.ClearTimer(ProjectileDestructionionTimerHandle);
}

