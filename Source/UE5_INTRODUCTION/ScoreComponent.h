// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Goal/Goal.h"
#include "ScoreComponent.generated.h"

USTRUCT(BlueprintType)
struct FTeamStrcut
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETeam GoalTeam = ETeam::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Score = 0;
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_INTRODUCTION_API UScoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UScoreComponent();

protected:
	TArray<FTeamStrcut> Scores;
	//TStaticArray<FTeamStrcut, ETeam::Max> TeamStatic;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;	

	TArray<AActor*> Goals;

	void SetScore(ETeam team);
};
