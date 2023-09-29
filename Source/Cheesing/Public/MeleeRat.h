// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Components/SphereComponent.h>
#include "SIntermediaryWaypoint.h"

#include "MeleeRat.generated.h"

UCLASS()
class CHEESING_API AMeleeRat : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = "Awareness")
	bool isPlayerDeteced;
	UPROPERTY(VisibleAnywhere, Category = "Awareness")
	bool canAttackPlayer;

	class ACheesingCharacter* playerRef;

	// Sets default values for this character's properties
	AMeleeRat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = "Radius")
	USphereComponent* detectionRadius;

	UPROPERTY(BlueprintReadWrite, Category = "Radius")
	USphereComponent* attackDetectionRadius;

	class AMeleeRatAIControler* controllerRef;

	

	UPROPERTY(EditAnywhere, Category = "Patrol")
	TArray<USIntermediaryWaypoint*> patrolWaypoints;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnAIMoveCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);


};
