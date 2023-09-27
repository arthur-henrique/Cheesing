// Fill out your copyright notice in the Description page of Project Settings.


#include "FixedTrampoline.h"

// Sets default values
AFixedTrampoline::AFixedTrampoline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isActive = false;
}

// Called when the game starts or when spawned
void AFixedTrampoline::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFixedTrampoline::Warp(float DeltaTime)
{
	timeDelta += DeltaTime;
	timeDelta /= speedDivider;

	playerActor->SetActorLocation(FMath::Lerp(playerActor->GetActorLocation(), endPoint->GetComponentLocation(), timeDelta));

	if (FMath::IsNearlyEqual(timeDelta, 1.f, .1f))
	{
		timeDelta = 0;
		isActive = false;
		playerActor->GetCharacterMovement()->GravityScale = normalGravity;
	}
}

// Called every frame
void AFixedTrampoline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isActive)
	{
		Warp(DeltaTime);
	}
}

void AFixedTrampoline::Interact()
{
	isActive = true;
	normalGravity = playerActor->GetCharacterMovement()->GravityScale;
	playerActor->GetCharacterMovement()->GravityScale = 0;
}

