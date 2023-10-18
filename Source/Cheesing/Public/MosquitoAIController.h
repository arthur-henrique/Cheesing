// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SIntermediaryWaypoint.h"


#include "MosquitoAIController.generated.h"

UCLASS()
class CHEESING_API AMosquitoAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	int currentWaypointIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	TArray<USIntermediaryWaypoint*> patrolWaypoints;

	void BeginPlay() override;

public:
	UFUNCTION()
	void Patrol();

	UFUNCTION()
	void GetWaypoints(TArray<USIntermediaryWaypoint*> waypoints);
};
