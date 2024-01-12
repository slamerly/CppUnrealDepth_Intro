// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ThrowForceDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UE5_INTRODUCTION_API UThrowForceDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "ThrowForce")
	float MinThrowForce = 500.f;
	UPROPERTY(EditAnywhere, Category = "ThrowForce")
	float MaxThrowForce = 5000.f;
	UPROPERTY(EditAnywhere, Category = "ThrowForce")
	float TimeToReachMaxThrowForce = 3.f;
};
