// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SpawnerPickUpComponent.h"
#include "Component/GravityGunComponent.h"
#include "Character/MyCharacter.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
USpawnerPickUpComponent::USpawnerPickUpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawnerPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickUp::StaticClass(), PickUps);
	if (PickUps.Num() != 0)
	{
		UE_LOG(LogTemp, Log, TEXT("there's no goal in the map"));
		for (AActor* PickUpActor : PickUps)
		{
			APickUp* PickUp = Cast<APickUp>(PickUpActor);
			if (PickUp)
			{
				switch (PickUp->GetPickUpType())
				{
				case EPickUpType::Normal:
					CurrentPickUpNormal++;
					break;
				case EPickUpType::DestroyAfterPickUp:
					CurrentPickUpDATake++;
					break;
				case EPickUpType::DestroyAfterThrow:
					CurrentPickUpDAThrow++;
					break;
				default:
					break;
				}
				UpdateNumberPickUp();
			}
		}
	}
	
}


// Called every frame
void USpawnerPickUpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimerSpawn += DeltaTime;
}

void USpawnerPickUpComponent::OnSpawnPickUpNormalInputPressed()
{
	SpawnPickUp(EPickUpType::Normal);
}

void USpawnerPickUpComponent::OnSpawnPickUpDATakeInputPressed()
{
	SpawnPickUp(EPickUpType::DestroyAfterPickUp);
}

void USpawnerPickUpComponent::OnSpawnPickUpDAThrowInputPressed()
{
	SpawnPickUp(EPickUpType::DestroyAfterThrow);
}

void USpawnerPickUpComponent::OnCountPickUpInputPressed()
{
	UpdateNumberPickUp();
	UE_LOG(LogTemp, Log, TEXT("Global number of PickUp: %d\nNumber of Normal PickUp: %d\nNumber of PickUp Destroy after PickUp: %d\nNumber of PickUp Destroy after throw: %d"), CurrentPickUpGlobal, CurrentPickUpNormal, CurrentPickUpDATake, CurrentPickUpDAThrow);
}

void USpawnerPickUpComponent::OnDestroyPickUpInputPressed()
{
	APickUp* PickUpToDesrtoy = GravityGunComponent->GetCurrentPickUp();
	if (PickUpToDesrtoy)
	{
		switch (PickUpToDesrtoy->GetPickUpType())
		{
		case EPickUpType::Normal:
			CurrentPickUpNormal--;
			break;
		case EPickUpType::DestroyAfterPickUp:
			CurrentPickUpDATake--;
			break;
		case EPickUpType::DestroyAfterThrow:
			CurrentPickUpDAThrow--;
			break;
		default:
			break;
		}
		GravityGunComponent->PickUpDestroy();
		UpdateNumberPickUp();
	}
}

void USpawnerPickUpComponent::SetCharacter(AMyCharacter* InCharacter)
{
	Character = InCharacter;
}

void USpawnerPickUpComponent::SetGravityGunComponent(UGravityGunComponent* InGravityGunComponent)
{
	GravityGunComponent = InGravityGunComponent;
}

void USpawnerPickUpComponent::SpawnPickUp(EPickUpType InPickUpType)
{
	if (!PickUpNormalClass)
	{
		UE_LOG(LogTemp, Log, TEXT("Pickup Class not define"));
		return;
	}

	if (TimerSpawn > CoolDownBetweenSpawn && CurrentPickUpGlobal < PickUpGlobalMax)
	{
		UE_LOG(LogTemp, Log, TEXT("Spawn Pickup"));

		switch (InPickUpType)
		{
		case EPickUpType::Normal:
			if (CurrentPickUpNormal < PickUpNormalMax)
			{
				if (GravityGunComponent->GetCurrentPickUp())
				{
					FVector PositionSpawn = CameraManager->GetCameraLocation() + CameraManager->GetActorForwardVector() * DistanceToSpawn;
					AActor* CurrentPickUp = GetWorld()->SpawnActor<AActor>(PickUpNormalClass, PositionSpawn, Character->GetActorRotation());
				}
				else
				{
					FVector PositionSpawn = CameraManager->GetCameraLocation() + CameraManager->GetActorForwardVector() * DistanceToSpawn;
					AActor* CurrentPickUpActor = GetWorld()->SpawnActor<AActor>(PickUpNormalClass, PositionSpawn, Character->GetActorRotation());

					APickUp* CurrentPickUp = Cast<APickUp>(CurrentPickUpActor);
					if (!CurrentPickUp)
						return;

					GravityGunComponent->SpawnInHand(CurrentPickUp);
				}
				CurrentPickUpNormal++;
			}
			break;
		case EPickUpType::DestroyAfterThrow:
			if (CurrentPickUpDAThrow < PickUpDAThrowMax)
			{
				if (GravityGunComponent->GetCurrentPickUp())
				{
					FVector PositionSpawn = CameraManager->GetCameraLocation() + CameraManager->GetActorForwardVector() * DistanceToSpawn;
					AActor* CurrentPickUp = GetWorld()->SpawnActor<AActor>(PickUpDAThrowClass, PositionSpawn, Character->GetActorRotation());
				}
				else
				{
					FVector PositionSpawn = CameraManager->GetCameraLocation() + CameraManager->GetActorForwardVector() * DistanceToSpawn;
					AActor* CurrentPickUpActor = GetWorld()->SpawnActor<AActor>(PickUpDAThrowClass, PositionSpawn, Character->GetActorRotation());

					APickUp* CurrentPickUp = Cast<APickUp>(CurrentPickUpActor);
					if (!CurrentPickUp)
						return;

					GravityGunComponent->SpawnInHand(CurrentPickUp);
				}
				CurrentPickUpDAThrow++;
			}
			break;
		case EPickUpType::DestroyAfterPickUp:
			if (CurrentPickUpDATake < PickUpDATakeMax)
			{
				if (GravityGunComponent->GetCurrentPickUp())
				{
					FVector PositionSpawn = CameraManager->GetCameraLocation() + CameraManager->GetActorForwardVector() * DistanceToSpawn;
					AActor* CurrentPickUp = GetWorld()->SpawnActor<AActor>(PickUpDATakeClass, PositionSpawn, Character->GetActorRotation());
				}
				else
				{
					FVector PositionSpawn = CameraManager->GetCameraLocation() + CameraManager->GetActorForwardVector() * DistanceToSpawn;
					AActor* CurrentPickUpActor = GetWorld()->SpawnActor<AActor>(PickUpDATakeClass, PositionSpawn, Character->GetActorRotation());

					APickUp* CurrentPickUp = Cast<APickUp>(CurrentPickUpActor);
					if (!CurrentPickUp)
						return;

					GravityGunComponent->SpawnInHand(CurrentPickUp);
				}
				CurrentPickUpDATake++;
			}
			break;
		case EPickUpType::Max:
			break;
		default:
			break;
		}

		//UpdateNumberPickUp();
		//UE_LOG(LogTemp, Log, TEXT("1: %d, 2: %d, 3: %d"), CurrentPickUpNormal, CurrentPickUpDAThrow, CurrentPickUpDATake);

		TimerSpawn = 0;
	}
	
	UpdateNumberPickUp();
}

void USpawnerPickUpComponent::UpdateNumberPickUp()
{
	CurrentPickUpGlobal = CurrentPickUpNormal + CurrentPickUpDATake + CurrentPickUpDAThrow;
	if (CurrentPickUpGlobal >= PickUpGlobalMax)
	{
		UE_LOG(LogTemp, Log, TEXT("Number of PickUp Max in the level!"));
	}
}