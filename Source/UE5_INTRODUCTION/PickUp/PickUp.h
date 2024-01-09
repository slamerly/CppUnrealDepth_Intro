// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUp.generated.h"

UENUM()
enum class EPickUpType : uint8
{
	Normal,
	DestroyAfterThrow,
	DestroyAfterPickUp,
	Max UMETA(Hidden),
};

USTRUCT(BlueprintType)
struct FPickUpStrcut
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPickUpType PickUpType = EPickUpType::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1.0", ClampMax = "10.0", EditCondition = "PickUpType != EPickUpType::Normal", EditConditionHides))
	float DestructionTimer = 5.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickUpDestroyDelegate);

UCLASS( Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class UE5_INTRODUCTION_API APickUp : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Pick Up")
	FPickUpStrcut PickUpStruct;

public:	
	// Sets default values for this actor's properties
	APickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Detonation
protected:
	FTimerHandle ProjectileDestructionionTimerHandle;

public:
	void StartPickUpDetonationTimer();
	void DestroyPickUp();
	EPickUpType GetPickUpType();
	void ClearTimer();

	FOnPickUpDestroyDelegate OnPickUpDestroy;

};
