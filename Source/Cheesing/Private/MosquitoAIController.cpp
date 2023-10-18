// Fill out your copyright notice in the Description page of Project Settings.


#include "MosquitoAIController.h"

void AMosquitoAIController::BeginPlay()
{
	Super::BeginPlay();

	currentWaypointIndex = 0;
}

void AMosquitoAIController::Patrol()
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

void AMosquitoAIController::GetWaypoints(TArray<USIntermediaryWaypoint*> waypoints)
{
	patrolWaypoints = waypoints;
}
