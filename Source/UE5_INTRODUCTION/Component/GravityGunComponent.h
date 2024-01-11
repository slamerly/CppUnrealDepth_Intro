// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityGunComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickUpTakenDelegate, FString, PickUpName);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_INTRODUCTION_API UGravityGunComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGravityGunComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnTakeObjectInputPressed();
	void OnThrowObjectInputPressed();
	void OnThrowObjectInputReleased();
	void OnUpSizeRaycastInputPressed();
	void SetCharacter(class AMyCharacter* InCharacter);
		

	//Collision
	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision")
	TEnumAsByte<ETraceTypeQuery> SimpleCollisionTraceChannel;
	ECollisionChannel GravityGunCollisionChannel;

	float ClampMinRaycast = 0.f;
	float ClampMaxRaycast = 3000.f;

	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision", meta = (ClampMin = "0.0", ClampMax = "3000.0"))
	float RaycastSize = 500.f;
	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float ScaleUpRycast = 50.f;

	class AMyCharacter* Character = nullptr;
	TWeakObjectPtr<class APlayerCameraManager> CameraManager = nullptr;

	//Pick up
protected:
	class APickUp* CurrentPickUp = nullptr;
	UStaticMeshComponent* PickUpCube = nullptr;
	FName PreviousCollisionProfile = NAME_None; // comme nullptr

	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision", meta = (ClampMin = "0.0", ClampMax = "3000.0"))
	float PickUpDistanceFromPlayer = 200.f;
	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision", meta = (ClampMin = "0.0", ClampMax = "10000.0"))
	float PickUpThrowForce = 2000.f;
	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision")
	FVector PickUpAngularForce = FVector(5000.f);

	// event on pick up destroy
protected:
	UFUNCTION()
	void OnHoldPickUpDestroyed();

	//exo2
	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float DurationMaxThrowHolding = 5.f;
	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision", meta = (ClampMin = "1.0", ClampMax = "5.0"))
	float PickUpThrowForceMax = 50000.f;

	bool bHoldingThrow = false;
	float TimerHoldingTrow = 0.f;
	float CurrentThrowForce = 1.f;

protected:
	void UpdatePickUpLocation();
	void ReleasePickUp(bool bThrow = false);

	// exemple - event in bp
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FPickUpTakenDelegate OnPickUpTake;

	//Debug
protected:
	UPROPERTY(EditAnywhere, Category = "GravityGun| Debug")
	bool bDrawDebugRaycast = false;
	UPROPERTY(EditAnywhere, Category = "GravityGun| Debug", meta = (ClampMin = "0.1", ClampMax = "30.0"))
	float TimerDebugRaycast = 5.f;

public:
	APickUp* GetCurrentPickUp() { return CurrentPickUp; }
	void SpawnInHand(APickUp* InPickUp);
	void PickUpDestroy();
};
