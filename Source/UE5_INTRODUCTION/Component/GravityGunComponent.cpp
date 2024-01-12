// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/GravityGunComponent.h"
#include "Kismet/Gameplaystatics.h"
#include "Character/MyCharacter.h"
#include "Curves/CurveFloat.h"
#include "PickUp/PickUp.h"
#include "DataAsset/ThrowForceDataAsset.h"

// Sets default values for this component's properties
UGravityGunComponent::UGravityGunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGravityGunComponent::BeginPlay()
{
	Super::BeginPlay();

	CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	GravityGunCollisionChannel = UEngineTypes::ConvertToCollisionChannel(SimpleCollisionTraceChannel);
	
}


// Called every frame
void UGravityGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatePickUpLocation();

	if (bHoldingThrow && TimerHoldingTrow < DurationMaxThrowHolding)
		TimerHoldingTrow += DeltaTime;
	
}

void UGravityGunComponent::OnTakeObjectInputPressed()
{
	// check if pick up present
	if (CurrentPickUp)
	{
		ReleasePickUp();
		return;
	}

	//UE_LOG(LogTemp, Log, TEXT("Take object pressed"));
	FVector RaycastStart = CameraManager->GetCameraLocation();
	FVector RaycastEnd = RaycastStart + CameraManager->GetActorForwardVector() * RaycastSize;
	 
	// Params
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);
	FHitResult RaycastHit;

	bool bHit = GetWorld()->LineTraceSingleByChannel(RaycastHit, RaycastStart, RaycastEnd, 
		GravityGunCollisionChannel, Params);

#if !UE_BUILD_SHIPPING
	if (bDrawDebugRaycast)
		DrawDebugLine(GetWorld(), RaycastStart, RaycastEnd, FColor::Red, false, TimerDebugRaycast, 0, 3.5f);
#endif

	if(!bHit)
		UE_LOG(LogTemp, Log, TEXT("hit nothing"));

	// check type
	CurrentPickUp = Cast<APickUp>(RaycastHit.GetActor());
	if (!CurrentPickUp)
		return;
	UE_LOG(LogTemp, Log, TEXT("hit %s"), *CurrentPickUp->GetName());


	// Get the mesh
	PickUpCube = CurrentPickUp->GetComponentByClass<UStaticMeshComponent>();
	if (!PickUpCube)
		return;


	UE_LOG(LogTemp, Log, TEXT("ets %s"), *CurrentPickUp->GetName());
	//disable physics
	PickUpCube->SetSimulatePhysics(false);

	//Update collision profile
	PreviousCollisionProfile = PickUpCube->GetCollisionProfileName();
	PickUpCube->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	

	// Reset timer if required
	if(CurrentPickUp->GetPickUpType() != EPickUpType::Normal)
		CurrentPickUp->ClearTimer();

	// chek if destruction timer required
	if (CurrentPickUp->GetPickUpType() == EPickUpType::DestroyAfterPickUp)
	{
		UE_LOG(LogTemp, Log, TEXT("test"));
		CurrentPickUp->StartPickUpDetonationTimer();

		CurrentPickUp->OnPickUpDestroy.AddUniqueDynamic(this, &UGravityGunComponent::OnHoldPickUpDestroyed);
	}

	UE_LOG(LogTemp, Log, TEXT("static mesh found"));

	OnPickUpTake.Broadcast(CurrentPickUp->GetName());
}

void UGravityGunComponent::OnThrowObjectInputPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Throw object pressed"));
	if (!PickUpCube)
		return;

	bHoldingThrow = true;

	//ReleasePickUp(true);
}

void UGravityGunComponent::OnThrowObjectInputReleased()
{
	UE_LOG(LogTemp, Log, TEXT("Throw object released : %f"), TimerHoldingTrow);
	if (!PickUpCube)
		return;

	CurrentThrowForce = PickUpThrowForce * TimerHoldingTrow;
	float SaveBasePickUpThrowForce = PickUpThrowForce;

	if (TimerHoldingTrow >= 2 && CurrentThrowForce < PickUpThrowForceMax)
	{
		PickUpThrowForce = CurrentThrowForce;
	}

	ReleasePickUp(true);

	PickUpThrowForce = SaveBasePickUpThrowForce;
	bHoldingThrow = false;
	TimerHoldingTrow = 0.f;
}

void UGravityGunComponent::OnUpSizeRaycastInputPressed()
{
	if (RaycastSize < ClampMaxRaycast)
		RaycastSize += ScaleUpRycast;
	else
		RaycastSize = ClampMinRaycast;
	UE_LOG(LogTemp, Log, TEXT("Raycast size %f"), RaycastSize);
}

void UGravityGunComponent::SetCharacter(AMyCharacter* InCharacter)
{
	Character = InCharacter;
}

float UGravityGunComponent::GetTimeToReachMaxThrowForce()
{
	return ThrowForceDataAsset->TimeToReachMaxThrowForce;
}

float UGravityGunComponent::GetCurrentTimeToReachMaxThrowForce()
{
	return TimerHoldingTrow;
}

void UGravityGunComponent::OnHoldPickUpDestroyed()
{
	//CurrentPickUp->OnPickUpDestroy.RemoveDynamic(this, &UGravityGunComponent::OnHoldPickUpDestroyed);

	ReleasePickUp();
}

void UGravityGunComponent::UpdatePickUpLocation()
{
	if (!PickUpCube)
	{
		UE_LOG(LogTemp, Log, TEXT("upLoc"), RaycastSize);
		return;

	}

	

	FVector NewPickUpLocation = CameraManager->GetCameraLocation() + CameraManager->GetActorForwardVector() * PickUpDistanceFromPlayer;
	CurrentPickUp->SetActorLocationAndRotation(NewPickUpLocation, CameraManager->GetActorQuat());
}

void UGravityGunComponent::ReleasePickUp(bool bThrow)
{
	if(CurrentPickUp->GetPickUpType() == EPickUpType::DestroyAfterPickUp)
		CurrentPickUp->OnPickUpDestroy.RemoveDynamic(this, &UGravityGunComponent::OnHoldPickUpDestroyed);

	if (PickUpCube)
	{
		// set back coll profile
		PickUpCube->SetCollisionProfileName(PreviousCollisionProfile);

		// set physics active
		PickUpCube->SetSimulatePhysics(true);

		// if throw the pick up
		if (bThrow)
		{
			float ThrowForce = 0.0f;
			if (ThrowForceCurve)
			{
				ThrowForce = ThrowForceCurve->GetFloatValue(TimerHoldingTrow);
			}
			else
			{
				if (ThrowForceDataAsset)
				{
					float ThrowForceAlpha = FMath::Clamp(TimerHoldingTrow / ThrowForceDataAsset->TimeToReachMaxThrowForce, 0.f, 1.f);
					ThrowForce = FMath::Lerp(ThrowForceDataAsset->MinThrowForce, ThrowForceDataAsset->MaxThrowForce, ThrowForceAlpha);
					UE_LOG(LogTemp, Log, TEXT("Throw Force Alpha %f - throw force %f"), ThrowForceAlpha, ThrowForce);
				}
				else
				{

				}
			}
			FVector Impulse = CameraManager->GetActorForwardVector() * PickUpThrowForce;
			PickUpCube->AddImpulse(Impulse);
			FVector AngularImpulse = FVector(FMath::RandRange(.0, PickUpAngularForce.X), FMath::RandRange(.0, PickUpAngularForce.Y), FMath::RandRange(.0, PickUpAngularForce.Z));
			PickUpCube->AddAngularImpulseInDegrees(AngularImpulse);
		}

		if (CurrentPickUp->GetPickUpType() == EPickUpType::DestroyAfterThrow)
			CurrentPickUp->StartPickUpDetonationTimer();
	}
	// clean refs
	CurrentPickUp = nullptr;
	PickUpCube = nullptr;
}

void UGravityGunComponent::SpawnInHand(APickUp* InPickUp)
{
	
	CurrentPickUp = InPickUp;
	// Get the mesh
	PickUpCube = CurrentPickUp->GetComponentByClass<UStaticMeshComponent>();
	if (!PickUpCube)
		return;

	//disable physics
	PickUpCube->SetSimulatePhysics(false);

	//Update collision profile
	PreviousCollisionProfile = PickUpCube->GetCollisionProfileName();
	PickUpCube->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);


	// Reset timer if required
	if (CurrentPickUp->GetPickUpType() != EPickUpType::Normal)
		CurrentPickUp->ClearTimer();

	// chek if destruction timer required
	if (CurrentPickUp->GetPickUpType() == EPickUpType::DestroyAfterPickUp)
	{
		UE_LOG(LogTemp, Log, TEXT("test"));
		CurrentPickUp->StartPickUpDetonationTimer();

		CurrentPickUp->OnPickUpDestroy.AddUniqueDynamic(this, &UGravityGunComponent::OnHoldPickUpDestroyed);
	}
	OnPickUpTake.Broadcast(CurrentPickUp->GetName());
}

void UGravityGunComponent::PickUpDestroy()
{
	PickUpCube = nullptr;
	CurrentPickUp->Destroy();
	CurrentPickUp = nullptr;
}

