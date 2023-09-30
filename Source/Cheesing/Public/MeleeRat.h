// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Components/SphereComponent.h>
#include "SIntermediaryWaypoint.h"
#include "DamageInterface.h"
#include <Cheesing/CheesingCharacter.h>

#include "MeleeRat.generated.h"

UCLASS()
class CHEESING_API AMeleeRat : public ACharacter, public IDamageInterface
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

	UFUNCTION(BlueprintCallable, Category = "HeadJump")
	void LaunchPlayerUp();

	virtual void TakeDamageM(int damage) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnAIMoveCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);

protected:

	UPROPERTY(EditAnywhere, Category = "Health")
	int health;

	UPROPERTY(EditAnywhere, Category = "HeadJump")
	float upForce;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "HeadJump")
	USphereComponent* headRadius;

};
