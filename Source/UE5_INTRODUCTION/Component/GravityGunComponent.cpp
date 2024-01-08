// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/GravityGunComponent.h"
#include "Kismet/Gameplaystatics.h"
#include "Character/MyCharacter.h"

// Sets default values for this component's properties
UGravityGunComponent::UGravityGunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

	// ...
}

void UGravityGunComponent::OnTakeObjectInputPressed()
{
	//UE_LOG(LogTemp, Log, TEXT("Take object pressed"));
	FVector RaycastStart = CameraManager->GetCameraLocation();
	FVector RaycastEnd = RaycastStart + CameraManager->GetActorForwardVector() + RaycastSize;
	 
	// Params
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);
	FHitResult RaycastHit;

	bool bHit = GetWorld()->LineTraceSingleByChannel(RaycastHit, RaycastStart, RaycastEnd, 
		GravityGunCollisionChannel, Params);

	if(bHit)
		UE_LOG(LogTemp, Log, TEXT("hit a pickup"));

}

void UGravityGunComponent::OnThrowObjectInputPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Throw object pressed"));
}

void UGravityGunComponent::SetCharacter(AMyCharacter* InCharacter)
{
	Character = InCharacter;
}

