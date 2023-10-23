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
	AMeleeRatAIControler();

	void BeginPlay() override;

	void Tick(float deltaTime) override;

private:

	float timer;

	class UNavigationSystemV1* NavArea;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	TArray<USIntermediaryWaypoint*> patrolWaypoints;

public:

	UPROPERTY(VisibleAnywhere)
	int currentWaypointIndex = 0;

	UFUNCTION(BlueprintCallable)
	void Patrol();

	UFUNCTION()
	void GetWaypoints(TArray<USIntermediaryWaypoint*> waypoints);
};
