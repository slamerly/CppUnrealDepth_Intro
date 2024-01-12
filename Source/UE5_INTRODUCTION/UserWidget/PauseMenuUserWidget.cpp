// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/PauseMenuUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Controller/MyPlayerController.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/Slider.h"

void UPauseMenuUserWidget::OpenMenu()
{
	SetVisibility(ESlateVisibility::Visible);
	AddToViewport(0);

	if (PlayerController)
	{
		PlayerController->SetShowMouseCursor(true);
		PlayerController->SetInputMode(FInputModeUIOnly{});
	}

	UGameplayStatics::SetGamePaused(this, true);
}

void UPauseMenuUserWidget::Resume()
{
	SetVisibility(ESlateVisibility::Collapsed);
	RemoveFromParent();

	if (PlayerController)
	{
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetInputMode(FInputModeGameOnly{});
	}

	UGameplayStatics::SetGamePaused(this, false);
}

void UPauseMenuUserWidget::Restart()
{
	FName LevelName = FName(*UGameplayStatics::GetCurrentLevelName(this));
	UGameplayStatics::OpenLevel(this, LevelName);

	if (!PlayerController)
	{
		PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	}
	PlayerController->SetInputMode(FInputModeGameOnly{});
}

void UPauseMenuUserWidget::Quit()
{
	UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, true);
}

void UPauseMenuUserWidget::OpenPauseMenu()
{
	Main_VerticalBox->SetVisibility(ESlateVisibility::Collapsed);
	Option_VerticalBox->SetVisibility(ESlateVisibility::Visible);
}

void UPauseMenuUserWidget::ReturnMainMenu()
{
	Main_VerticalBox->SetVisibility(ESlateVisibility::Visible);
	Option_VerticalBox->SetVisibility(ESlateVisibility::Collapsed);
}

void UPauseMenuUserWidget::RecieveXSliderValue(float NewValue)
{
	if (PlayerController)
		PlayerController->SetXSensitivity(NewValue);
}

void UPauseMenuUserWidget::RecieveYSliderValue(float NewValue)
{
	if (PlayerController)
		PlayerController->SetYSensitivity(NewValue);
}

void UPauseMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	OpenMenu();

	if (ButtonResume)
		ButtonResume->OnClicked.AddUniqueDynamic(this, &UPauseMenuUserWidget::Resume);
		//UE_LOG(LogTemp, Log, TEXT("ButtonResume exeec"));
	if (ButtonRestart)
		ButtonRestart->OnClicked.AddUniqueDynamic(this, &UPauseMenuUserWidget::Restart);
		//UE_LOG(LogTemp, Log, TEXT("ButtonRestart exeec"));
	if (ButtonQuit)
		ButtonQuit->OnClicked.AddUniqueDynamic(this, &UPauseMenuUserWidget::Quit);
		//UE_LOG(LogTemp, Log, TEXT("ButtonQuit exeec"));

	if (ButtonOption)
		ButtonOption->OnClicked.AddUniqueDynamic(this, &UPauseMenuUserWidget::OpenPauseMenu);
	//UE_LOG(LogTemp, Log, TEXT("ButtonRestart exeec"));
	if (ButtonReturnMainMenu)
		ButtonReturnMainMenu->OnClicked.AddUniqueDynamic(this, &UPauseMenuUserWidget::ReturnMainMenu);
	//UE_LOG(LogTemp, Log, TEXT("ButtonQuit exeec"));

	ReturnMainMenu();

	if (XSlider)
	{
		XSlider->OnValueChanged.AddUniqueDynamic(this, &UPauseMenuUserWidget::RecieveXSliderValue);
		XSlider->SetValue(PlayerController->GetXSensitivity());
	}
	if (YSlider)
	{
		YSlider->OnValueChanged.AddUniqueDynamic(this, &UPauseMenuUserWidget::RecieveYSliderValue);
		YSlider->SetValue(PlayerController->GetYSensitivity());
	}
}
