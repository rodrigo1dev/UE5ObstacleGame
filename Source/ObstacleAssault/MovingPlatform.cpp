// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Character.h" // for LaunchCharacter

// Sets default values
AMovingPlatform::AMovingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();

	// Bind overlap events if jumping is enabled
	if (bCanJump)
	{
		// Use the actor's overlap delegates
		OnActorBeginOverlap.AddDynamic(this, &AMovingPlatform::OnOverlapBegin);
		OnActorEndOverlap.AddDynamic(this, &AMovingPlatform::OnOverlapEnd);
	}
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlatform(DeltaTime);
	RotatePlatform(DeltaTime);

	// If there is an overlapping character, trigger launch when the player presses jump (rising edge)
	if (OverlappingCharacterRef.IsValid() && !bLaunchedThisOverlap)
	{
		ACharacter* Char = OverlappingCharacterRef.Get();
		bool bCurrentJumpPressed = Char->bPressedJump;
		if (bCurrentJumpPressed && !bPrevJumpPressed)
		{
			FVector LaunchVelocity = FVector(0.0f, 0.0f, JumpHeight);
			Char->LaunchCharacter(LaunchVelocity, false, true);
			bLaunchedThisOverlap = true;
			OverlappingCharacterRef.Reset();
			bPrevJumpPressed = false;
		}
		else
		{
			bPrevJumpPressed = bCurrentJumpPressed;
		}
	}
}

// Move the platform in the direction of PlatformVelocity
void AMovingPlatform::MovePlatform(float DeltaTime)
{
	DistanceMoved = GetDistanceMoved();

	if (DistanceMoved >= MoveDistance)
	{
		FVector MoveDirection = PlatformVelocity.GetSafeNormal();
		FVector NewStartLocation = StartLocation + MoveDirection * MoveDistance;

		SetActorLocation(NewStartLocation);
		StartLocation = NewStartLocation;

		PlatformVelocity = -PlatformVelocity;
	}
	else
	{
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation += PlatformVelocity * DeltaTime;
		SetActorLocation(CurrentLocation);
	}
}

// Rotate the platform over time
void AMovingPlatform::RotatePlatform(float DeltaTime)
{
	FRotator RotationToAdd = RotationVelocity * DeltaTime;
	AddActorLocalRotation(RotationToAdd);
}

// Get the distance moved from the start location
float AMovingPlatform::GetDistanceMoved()
{
	return FVector::Dist(StartLocation, GetActorLocation());
}

// Overlap begin: store the overlapping character to detect jump input
void AMovingPlatform::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!bCanJump) return;

	if (OtherActor && OtherActor != this)
	{
		ACharacter* OverlappingCharacter = Cast<ACharacter>(OtherActor);
		if (OverlappingCharacter)
		{
			OverlappingCharacterRef = OverlappingCharacter;
			bLaunchedThisOverlap = false;
			bPrevJumpPressed = OverlappingCharacter->bPressedJump; // initialize prev state
		}
	}
}

// Overlap end: clear stored reference
void AMovingPlatform::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	OverlappingCharacterRef.Reset();
	bLaunchedThisOverlap = false;
	bPrevJumpPressed = false;
}
