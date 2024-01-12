// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE5_INTRODUCTION_API UPauseMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OpenMenu();
	UFUNCTION()
	void Resume();
	UFUNCTION()
	void Restart();
	UFUNCTION()
	void Quit();

	UFUNCTION()
	void OpenPauseMenu();
	UFUNCTION()
	void ReturnMainMenu();
	
	UFUNCTION()
	void RecieveXSliderValue(float NewValue);
	UFUNCTION()
	void RecieveYSliderValue(float NewValue);

protected:
	class AMyPlayerController* PlayerController = nullptr;

protected:
	virtual void NativeConstruct() override;
	
	//button
	UPROPERTY(meta = (BindWdgetOptional))
	class UButton* ButtonResume;
	UPROPERTY(meta = (BindWdgetOptional))
	class UButton* ButtonRestart;
	UPROPERTY(meta = (BindWdgetOptional))
	class UButton* ButtonQuit;

	UPROPERTY(meta = (BindWdgetOptional))
	class UButton* ButtonOption;
	UPROPERTY(meta = (BindWdgetOptional))
	class UButton* ButtonReturnMainMenu;

	UPROPERTY(meta = (BindWdgetOptional))
	class UVerticalBox* Main_VerticalBox;
	UPROPERTY(meta = (BindWdgetOptional))
	class UVerticalBox* Option_VerticalBox;

	UPROPERTY(meta = (BindWdgetOptional))
	class USlider* XSlider;
	UPROPERTY(meta = (BindWdgetOptional))
	class USlider* YSlider;

	UPROPERTY(meta = (BindWdgetOptional))
	class UTextBlock* XSliderValue;
	UPROPERTY(meta = (BindWdgetOptional))
	class USlider* YSliderValue;
};
