// Fill out your copyright notice in the Description page of Project Settings.


#include "FixedTrampoline.h"
#include "..\Public\FixedTrampoline.h"

// Sets default values
AFixedTrampoline::AFixedTrampoline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isActive = false;
	currentPointIndex = 0;
}

// Called when the game starts or when spawned
void AFixedTrampoline::BeginPlay()
{
	Super::BeginPlay();
	
	GetComponents<USIntermediaryWaypoint>(intermediaryPoints);
	
	if (intermediaryPoints.Num() != 0)
	{
		for (int x = 0; x < intermediaryPoints.Num(); x++)
		{
			for (int i = 0; i < intermediaryPoints.Num(); i++)
			{
				if (intermediaryPoints[x]->position < intermediaryPoints[i]->position)
				{
					intermediaryPoints.Swap(x, i);
				}
			}
		}
	}
}

void AFixedTrampoline::Warp(float DeltaTime)
{
	timeDelta += DeltaTime;
	timeDelta /= speed;

	if (intermediaryPoints.Num() != 0) //Se os opontos intermediarios não forem nulos
	{
		playerActor->SetActorLocation(FMath::VInterpConstantTo(playerActor->GetActorLocation(), intermediaryPoints[currentPointIndex]->GetComponentLocation(), DeltaTime, speed));
	}

	

	if (FMath::IsNearlyEqual(timeDelta, 1.f, .8f))
	{
		
		if (intermediaryPoints.IsValidIndex(currentPointIndex + 1))
		{
			currentPointIndex++;
			timeDelta = 0;
		}
		else
		{
			timeDelta = 0;
			isActive = false;
			playerActor->GetCharacterMovement()->GravityScale = normalGravity;
			currentPointIndex = 0;
		}

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

void AFixedTrampoline::Interact_Implementation()
{
	isActive = true;
	normalGravity = playerActor->GetCharacterMovement()->GravityScale;
	playerActor->GetCharacterMovement()->GravityScale = 0;
}

