// Fill out your copyright notice in the Description page of Project Settings.


#include "Goal/Goal.h"
#include "Components/BoxComponent.h"
#include "ScoreComponent.h"
#include "PickUp/PickUp.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Character/MyCharacter.h>

// Sets default values
AGoal::AGoal(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// create collision box
	CollisionBox = ObjectInitializer.CreateOptionalDefaultSubobject<UBoxComponent>(this, TEXT("CollisionBox"));
	if (CollisionBox)
	{
		SetRootComponent(CollisionBox);
	}
}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AGoal::OnComponentBeginOverlap);

}

void AGoal::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APickUp* EnteringPickUp = Cast<APickUp>(OtherActor);
	if (!EnteringPickUp)
		return;

	Score++;
	UE_LOG(LogTemp, Log, TEXT("%s has a score of %d"), *GetName(), Score);
	OnPickUpInGoal.Broadcast(GoalTeam);
}

int AGoal::CountPickUpInGoal()
{
	// prepare raycast location
	FVector GoalLocation = GetActorLocation();
	FVector ScaledBoxExtend = CollisionBox->GetScaledBoxExtent();

	// launch our raycast
	const TArray<AActor*> ActorToIgnore;
	TArray<FHitResult> OutHits;
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), GoalLocation, GoalLocation, 
		ScaledBoxExtend, GetActorRotation(), GoalCollisionTraceChannel, false, 
		ActorToIgnore, EDrawDebugTrace::None, OutHits, true);

	int NumberOfPickUpInGoal = OutHits.Num();

	return NumberOfPickUpInGoal;
}

//void AGoal::SetCharacter(AMyCharacter* InCharacter)
//{
//	character = InCharacter;
//}

