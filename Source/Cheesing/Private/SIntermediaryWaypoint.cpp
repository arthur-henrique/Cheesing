// Fill out your copyright notice in the Description page of Project Settings.


#include "SIntermediaryWaypoint.h"

// Sets default values for this component's properties
USIntermediaryWaypoint::USIntermediaryWaypoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void USIntermediaryWaypoint::BeginPlay()
{
	Super::BeginPlay();

	// ...
	initialPosition = GetComponentLocation();
}


// Called every frame
void USIntermediaryWaypoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

