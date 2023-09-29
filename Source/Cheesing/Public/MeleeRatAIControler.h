// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SIntermediaryWaypoint.h"

#include "MeleeRatAIControler.generated.h"

/**
 * 
 */
UCLASS()
class CHEESING_API AMeleeRatAIControler : public AAIController
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;


private:

	UPROPERTY(VisibleAnywhere)
	int currentWaypointIndex = 0;

	class UNavigationSystemV1* NavArea;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	TArray<USIntermediaryWaypoint*> patrolWaypoints;

public:

	UFUNCTION()
	void Patrol();

	UFUNCTION()
	void GetWaypoints(TArray<USIntermediaryWaypoint*> waypoints);
};
