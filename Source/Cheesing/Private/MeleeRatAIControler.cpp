// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeRatAIControler.h"

#include "NavigationSystem.h"

void AMeleeRatAIControler::BeginPlay()
{
	Super::BeginPlay();

	currentWaypointIndex = 0;

	




	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this); //Gets current Navigation system for this object

	//Patrol();
}

void AMeleeRatAIControler::Patrol()
{
	if (NavArea)
	{
		if (patrolWaypoints.Num() > 0)
		{
			MoveToLocation(patrolWaypoints[currentWaypointIndex]->initialPosition, .1f);
			//UE_LOG(LogTemp, Display, TEXT("Funcionou"));
			currentWaypointIndex++;
		}
		//UE_LOG(LogTemp, Display, TEXT("Patrol"));

		
		if (!patrolWaypoints.IsValidIndex(currentWaypointIndex))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Chamou Crashou"));
			currentWaypointIndex = 0;
		}
	}
}

void AMeleeRatAIControler::GetWaypoints(TArray<USIntermediaryWaypoint*> waypoints)
{
	patrolWaypoints = waypoints;
}

