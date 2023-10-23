// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "SIntermediaryWaypoint.h"
#include "MosquitoAIController.h"
#include <Cheesing/CheesingCharacter.h>
#include <Components/SphereComponent.h>

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

	UPROPERTY(EditAnywhere, Category = "Patrol")
	TArray<USIntermediaryWaypoint*> patrolWaypoints;

	UPROPERTY(VisibleAnywhere, Category = "Patrol")
	int currentWaypointIndex;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float speed;

	UPROPERTY(EditAnywhere, Category = "Head Jump")
	float upForce;

	UFUNCTION(BlueprintCallable)
	void LaunchPlayerUp();

	UPROPERTY(BlueprintReadWrite)
	USphereComponent* headRadius;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	

};
