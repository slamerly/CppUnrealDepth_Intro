// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickUp/PickUp.h"
#include "SpawnerPickUpComponent.generated.h"



UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_INTRODUCTION_API USpawnerPickUpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnerPickUpComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void OnSpawnPickUpNormalInputPressed();
	void OnSpawnPickUpDATakeInputPressed();
	void OnSpawnPickUpDAThrowInputPressed();
	void OnCountPickUpInputPressed();
	void OnDestroyPickUpInputPressed();
	void SetCharacter(class AMyCharacter* InCharacter);
	void SetGravityGunComponent(class UGravityGunComponent* InGravityGunComponent);

	// params spawner
protected:
	UPROPERTY(EditAnywhere, Category = "Spawner|Timer", meta = (ClampMin = "0.0", ClampMax = "15.0"))
	float CoolDownBetweenSpawn = 3.f;

	UPROPERTY(EditAnywhere, Category = "Spawner|Distance", meta = (ClampMin = "100.0", ClampMax = "500.0"))
	float DistanceToSpawn = 200.f;

	UPROPERTY(EditAnywhere, Category = "Spawner|PickUp", meta = (ClampMin = "0", ClampMax = "30"))
	int PickUpGlobalMax = 3;

	UPROPERTY(EditAnywhere, Category = "Spawner|PickUp")
	UClass* PickUpNormalClass;
	UPROPERTY(EditAnywhere, Category = "Spawner|PickUp", meta = (ClampMin = "0", ClampMax = "10"))
	int PickUpNormalMax = 1;
	UPROPERTY(EditAnywhere, Category = "Spawner|PickUp")
	UClass* PickUpDATakeClass;
	UPROPERTY(EditAnywhere, Category = "Spawner|PickUp", meta = (ClampMin = "0", ClampMax = "10"))
	int PickUpDATakeMax = 1;
	UPROPERTY(EditAnywhere, Category = "Spawner|PickUp")
	UClass* PickUpDAThrowClass;
	UPROPERTY(EditAnywhere, Category = "Spawner|PickUp", meta = (ClampMin = "0", ClampMax = "10"))
	int PickUpDAThrowMax = 1;

	float TimerSpawn = 3.f;
	class AMyCharacter* Character = nullptr;
	class UGravityGunComponent* GravityGunComponent = nullptr;
	TWeakObjectPtr<class APlayerCameraManager> CameraManager = nullptr;
	TArray<AActor*> PickUps;

	int CurrentPickUpNormal = 0;
	int CurrentPickUpDATake = 0;
	int CurrentPickUpDAThrow = 0;
	int CurrentPickUpGlobal = 0;

	void SpawnPickUp(EPickUpType InPickUpType);

	void UpdateNumberPickUp();
};
