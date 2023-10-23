// Fill out your copyright notice in the Description page of Project Settings.

#include "Mosquito.h"
#include "AITypes.h"

#include "Navigation/PathFollowingComponent.h"

// Sets default values
AMosquito::AMosquito()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMosquito::BeginPlay()
{
	Super::BeginPlay();
	
	GetComponents<USIntermediaryWaypoint>(patrolWaypoints);

	if (patrolWaypoints.Num() != 0) //Sorting
	{
		for (int x = 0; x < patrolWaypoints.Num(); x++)
		{
			for (int i = 0; i < patrolWaypoints.Num(); i++)
			{
				if (patrolWaypoints[x]->position < patrolWaypoints[i]->position)
				{
					patrolWaypoints.Swap(x, i);
				}
			}
		}
	}

}

// Called every frame
void AMosquito::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!(patrolWaypoints[currentWaypointIndex]->initialPosition - GetActorLocation()).IsNearlyZero(.1f))
	{
		GetRootComponent()->SetWorldLocation(FMath::VInterpConstantTo(GetActorLocation(), patrolWaypoints[currentWaypointIndex]->initialPosition, DeltaTime, speed));
	}
	else
	{
		currentWaypointIndex++;
		if (!patrolWaypoints.IsValidIndex(currentWaypointIndex))
			currentWaypointIndex = 0;
	}
}

/**Launch Player upwards when enters a trigger*/
void AMosquito::LaunchPlayerUp()
{
	TSubclassOf<ACheesingCharacter> playerClass;
	TArray<AActor*> player;
	headRadius->GetOverlappingActors(player, playerClass);

	UE_LOG(LogTemp, Display, TEXT("Player Detected"));

	for (AActor* playerA : player)
	{
		if (ACheesingCharacter* cPlayer = Cast<ACheesingCharacter>(playerA))
		{
			cPlayer->LaunchCharacter(FVector(0, 0, upForce), false, true);
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald, TEXT("Launched"));
		}
	}
}


