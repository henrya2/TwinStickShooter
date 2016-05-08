// Fill out your copyright notice in the Description page of Project Settings.

#include "TwinStickShooter.h"
#include "BaseCharacter.h"


void ABaseCharacter::CalculateDead()
{
	if (Health <= 0)
		isDead = true;
	else
		isDead = false;
}

void ABaseCharacter::CalculateHealth(float Delta)
{
	Health += Delta;
	CalculateDead();
}

#if WITH_EDITOR
void ABaseCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	isDead = false;
	Health = 100;

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bEnableMouseControl = false;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (InputEnabled())
	{
		if (bEnableMouseControl)
		{
			APlayerController* PossessedPlayerController = Cast<APlayerController>(GetController());
			if (PossessedPlayerController)
			{
				static float MouseX = 0, MouseY = 0;
				PossessedPlayerController->GetMousePosition(MouseX, MouseY);
				FVector2D MouseLocation(MouseX, MouseY);
				FVector2D ActorScreenLocation(0, 0);

				PossessedPlayerController->ProjectWorldLocationToScreen(GetActorLocation(), ActorScreenLocation);
				FString Msg = FString::Printf(TEXT("ActorScreenLocation: (%f, %f)"), ActorScreenLocation.X, ActorScreenLocation.Y);
				GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, Msg);
				Msg = FString::Printf(TEXT("MouseLocation: (%f, %f)"), MouseX, MouseY);
				GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, Msg);

				FVector2D Dir(0, 0);
				Dir = MouseLocation - ActorScreenLocation;
				float Yaw = FMath::Atan2(Dir.X, -Dir.Y);
				Yaw = FMath::RadiansToDegrees(Yaw);

				Msg = FString::Printf(TEXT("Dir: (%f, %f), Yaw: %f"), Dir.X, Dir.Y, Yaw);
				GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, Msg);

				PossessedPlayerController->SetControlRotation(FRotator(0, Yaw, 0));
			}
		}
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxisKey(EKeys::MouseX, this, &ABaseCharacter::OnMouseX);
	InputComponent->BindAxisKey(EKeys::MouseY, this, &ABaseCharacter::OnMouseY);
}

void ABaseCharacter::OnMouseX(float Value)
{
	if (Value != 0)
	{
		FString Msg = FString::Printf(TEXT("MouseX: %f"), Value);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, Msg);
	}
}

void ABaseCharacter::OnMouseY(float Value)
{
	if (Value != 0)
	{
		FString Msg = FString::Printf(TEXT("MouseY: %f"), Value);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, Msg);
	}
}

