// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreComponent.h"
#include "Goal/Goal.h"
#include "Kismet/Gameplaystatics.h"

// Sets default values for this component's properties
UScoreComponent::UScoreComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UScoreComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Get All Goals
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoal::StaticClass(), Goals);
	if (Goals.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("there's no goal in the map"));
	}
	else
	{
		for (AActor* actorGoal : Goals)
		{
			AGoal* goal = Cast<AGoal>(actorGoal);
			if (goal)
			{
				goal->OnPickUpInGoal.AddUniqueDynamic(this, &UScoreComponent::SetScore);
			}
		}
	}

}

void UScoreComponent::SetScore(ETeam team)
{

	//Scores[team].score++;
}


