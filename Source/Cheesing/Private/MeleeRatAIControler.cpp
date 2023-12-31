// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeRatAIControler.h"
#include "NavigationSystem.h"

AMeleeRatAIControler::AMeleeRatAIControler()
{
	PrimaryActorTick.bCanEverTick = true;
	timer = 2;
}
void AMeleeRatAIControler::BeginPlay()
{
	Super::BeginPlay();

	currentWaypointIndex = 0;
	
	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this); //Gets current Navigation system for this object

	

	//Patrol();
}

void AMeleeRatAIControler::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if(timer < 2)
		timer += deltaTime;

}

void AMeleeRatAIControler::Patrol()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Patrolling");


	if (timer > 1)
	{
		if (NavArea)
		{


			if (!patrolWaypoints.IsValidIndex(currentWaypointIndex))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Chamou Crashou"));
				currentWaypointIndex = 0;
			}

			if (patrolWaypoints.Num() > 0)
			{
				MoveToLocation(patrolWaypoints[currentWaypointIndex]->initialPosition, .1f, true,true,true,true,0,true);
				//UE_LOG(LogTemp, Display, TEXT("Funcionou"));
				currentWaypointIndex++;
				
			}
			//UE_LOG(LogTemp, Display, TEXT("Patrol"));

		}

		timer = 0;
	}
}

void AMeleeRatAIControler::GetWaypoints(TArray<USIntermediaryWaypoint*> waypoints)
{
	patrolWaypoints = waypoints;
}

void AMeleeRatAIControler::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID,Result);

	if(!playerDetected && Result.IsSuccess())
		Patrol();
	else
	{
		//if (Result.IsFailure())
		//{
		//	//timerDelegate.BindUFunction(this, FName("OnMoveCompleted"), RequestID, Result);
		//	//UE_LOG(LogTemp, Warning, TEXT("Failed PathResult"));
		//	FTimerHandle callTimer;
		//	GetWorldTimerManager().SetTimer(callTimer,this,&AMeleeRatAIControler::Patrol, .2f, false);
		//}
			
	}

}

