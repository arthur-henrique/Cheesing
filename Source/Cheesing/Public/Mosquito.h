// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "SIntermediaryWaypoint.h"

#include "Mosquito.generated.h"

UCLASS()
class CHEESING_API AMosquito : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMosquito();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	UFloatingPawnMovement* movementComponent;

	class AMosquitoAIController* controllerRef;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	TArray<USIntermediaryWaypoint*> patrolWaypoints;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnAIMoveCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);
	

};
