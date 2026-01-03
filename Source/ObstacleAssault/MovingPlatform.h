// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

class ACharacter; // forward declaration

#include "MovingPlatform.generated.h"

UCLASS()
class OBSTACLEASSAULT_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Overlap callbacks
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MovePlatform(float DeltaTime);
	void RotatePlatform(float DeltaTime);
	float GetDistanceMoved();

	UPROPERTY(EditAnywhere)
	FVector PlatformVelocity = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(VisibleAnywhere)
	float DistanceMoved = 0.0f;

	UPROPERTY(EditAnywhere)
	float MoveDistance = 100.0f;	

	UPROPERTY(EditAnywhere)
	FRotator RotationVelocity;
	
	// Whether the platform should react to overlaps and perform a jump
	UPROPERTY(EditAnywhere, Category = "Jump")
	bool bCanJump = false;

	// Launch configuration (vertical velocity applied when player jumps on platform)
	UPROPERTY(EditAnywhere, Category = "Jump")
	float JumpHeight = 200.0f;

	FVector StartLocation;

private:
	// Keep a weak reference to the overlapping character so we can check its input while overlapping
	TWeakObjectPtr<ACharacter> OverlappingCharacterRef;
	// Ensure we only launch once per overlap
	bool bLaunchedThisOverlap = false;

	// Track previous jump input state to detect rising edge (player pressed jump while on platform)
	bool bPrevJumpPressed = false;


};
