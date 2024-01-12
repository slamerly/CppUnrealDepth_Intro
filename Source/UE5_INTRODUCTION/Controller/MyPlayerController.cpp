#include "Character/MyCharacter.h"
#include "Controller/MyPlayerController.h"
#include "Controller/GravityGunController.h"
#include "Controller/SpawnerController.h"
#include "Kismet/GameplayStatics.h"
#include "UserWidget/PauseMenuUserWidget.h"
#include "Goal/Goal.h"

void AMyPlayerController::MoveForward(float Value)
{
	//UE_LOG(LogTemp, Log, TEXT("Move Forward %f"), Value);

	if (Character && Value != 0.f)
		Character->AddMovementInput(Character->GetActorForwardVector(), Value);
}

void AMyPlayerController::MoveRight(float Value)
{
	//UE_LOG(LogTemp, Log, TEXT("Move Right %f"), Value);

	if (Character && Value != 0.f)
		Character->AddMovementInput(Character->GetActorRightVector(), Value);
}

void AMyPlayerController::SetupCameraComponent()
{
	InputComponent->BindAxis(LookUpInputName, Character, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis(LookRightInputName, Character, &APawn::AddControllerYawInput);
}

void AMyPlayerController::Jump()
{
	if (Character)
		Character->Jump();
}

void AMyPlayerController::CountScore()
{
	if (Goals.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("there's no goal in the map"));
	}

	for (AActor* CurrentGoal : Goals)
	{
		AGoal* Goal = Cast<AGoal>(CurrentGoal);
		if (Goal)
		{
			int PickUpInGoal = Goal->CountPickUpInGoal();
			UE_LOG(LogTemp, Log, TEXT("there's %d pick up in %s"), PickUpInGoal, *Goal->GetName());
		}
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind movement inputs
	InputComponent->BindAxis(MoveForwardInputName, this, &AMyPlayerController::MoveForward);
	InputComponent->BindAxis(MoveRightInputName, this, &AMyPlayerController::MoveRight);

	InputComponent->BindAction(JumpInputName, EInputEvent::IE_Pressed, this, &AMyPlayerController::Jump);
	InputComponent->BindAction(PauseMenuInputName, EInputEvent::IE_Pressed, this, &AMyPlayerController::OnPauseInputPressed);

	//bid score
	FInputActionBinding& ScoreInputActionBinding = InputComponent->BindAction(CountScoreInputName, EInputEvent::IE_Pressed, this, &AMyPlayerController::CountScore);
	ScoreInputActionBinding.bConsumeInput = false;
}

void AMyPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	Character = Cast<AMyCharacter>(InPawn);

	// bind camera
	SetupCameraComponent();

	//bind jump
	Jump();


}

float AMyPlayerController::GetXSensitivity()
{
	return MouseSensitivityX;
}

float AMyPlayerController::GetYSensitivity()
{
	return MouseSensitivityY;
}

void AMyPlayerController::SetXSensitivity(float NewSensitivity)
{
	MouseSensitivityX = NewSensitivity;
}

void AMyPlayerController::SetYSensitivity(float NewSensitivity)
{
	MouseSensitivityY = NewSensitivity;
}

void AMyPlayerController::AddPitchInput(float Val)
{
	float Multiplier = Val * MouseSensitivityY;
	Super::AddPitchInput(Multiplier);
}

void AMyPlayerController::AddYawInput(float Val)
{
	float Multiplier = Val * MouseSensitivityX;
	Super::AddYawInput(Multiplier);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->OnWorldBeginPlay.AddUObject(this, &AMyPlayerController::LateBeginPlay);

	// Get All Goals
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoal::StaticClass(), Goals);
	if (Goals.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("there's no goal in the map"));
	}
}

void AMyPlayerController::LateBeginPlay()
{
	GravityGunController = GetComponentByClass<UGravityGunController>();

	if (GravityGunController)
		GravityGunController->SetupInputComponentGravityGun(Character, InputComponent);

	SpwanerController = GetComponentByClass<USpawnerController>();

	if (SpwanerController)
		SpwanerController->SetupInputComponentSpawnerPickUp(Character, InputComponent);
}

void AMyPlayerController::OnPauseInputPressed()
{
	if (PauseMenuWidget)
	{
		//crea
		UPauseMenuUserWidget* MenuWidget = Cast<UPauseMenuUserWidget>(CreateWidget<UPauseMenuUserWidget>(this, PauseMenuWidget));
		if (MenuWidget)
		{
			MenuWidget->AddToViewport(0);
			//MenuWidget->OpenMenu();
		}
	}
}