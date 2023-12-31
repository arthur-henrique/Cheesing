// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Cheesing/CheesingCharacter.h>
#include "InteractionInterface.h"
#include <Components/SphereComponent.h>
#include "SIntermediaryWaypoint.h"

#include "FixedTrampoline.generated.h"

UCLASS()
class CHEESING_API AFixedTrampoline : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFixedTrampoline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Warp")
	bool isActive;

	UPROPERTY(EditAnywhere, Category = "Warp")
	float speed;

	UPROPERTY(VisibleAnywhere, Category = "Waypoints|Intermediary Waypoints")
	int currentPointIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Warp")
	ACheesingCharacter* playerActor;

	UFUNCTION(BlueprintCallable, Category = "Warp")
	void Warp(float DeltaTime);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Waypoints|Intermediary Waypoints")
	TArray<USIntermediaryWaypoint*> intermediaryPoints;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void Interact();
	virtual void Interact_Implementation() override;

private:
	float timeDelta;

	float normalGravity;
};
