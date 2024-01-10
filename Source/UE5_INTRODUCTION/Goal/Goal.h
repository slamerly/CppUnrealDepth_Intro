// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Goal.generated.h"

UENUM()
enum class ETeam : uint8
{
	None,
	TeamRed,
	TeamGreen,
	TeamBlue,
	Max UMETA(Hidden),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickUpInGoalDelegate, ETeam, TeamGoal);

UCLASS()
class UE5_INTRODUCTION_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// collision box
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* CollisionBox = nullptr;
	unsigned int Score = 0;
	UPROPERTY(EditAnywhere, Category = "Goal|Collision")
	TEnumAsByte<ETraceTypeQuery> GoalCollisionTraceChannel;

	// exo3 
protected:
	UPROPERTY(EditAnywhere, Category = "Goal|Team")
	ETeam GoalTeam = ETeam::None;
	AMyCharacter* character = nullptr;

public:
	UFUNCTION()
	void OnComponentBeginOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	int CountPickUpInGoal();

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FPickUpInGoalDelegate OnPickUpInGoal;
};
