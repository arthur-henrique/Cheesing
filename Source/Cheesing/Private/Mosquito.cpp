// Fill out your copyright notice in the Description page of Project Settings.

#include "AITypes.h"
#include "Mosquito.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
AMosquito::AMosquito()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMosquito::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMosquito::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AMosquito::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{

}


