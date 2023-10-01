// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeRat.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "MeleeRatAIControler.h"

// Sets default values
AMeleeRat::AMeleeRat()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMeleeRat::BeginPlay()
{
	Super::BeginPlay();

	controllerRef = Cast<AMeleeRatAIControler>(GetController());
	controllerRef->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AMeleeRat::OnAIMoveCompleted); //Notify when finished moving
	
	GetComponents<USIntermediaryWaypoint>(patrolWaypoints);

	if (patrolWaypoints.Num() != 0) //Sorting
	{
		for (int x = 0; x < patrolWaypoints.Num(); x++)
		{
			for (int i = 0; i < patrolWaypoints.Num(); i++)
			{
				if (patrolWaypoints[x]->position < patrolWaypoints[i]->position)
				{
					patrolWaypoints.Swap(x, i);
				}
			}
		}
	}

	controllerRef->GetWaypoints(patrolWaypoints);
	controllerRef->Patrol();

}

void AMeleeRat::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	controllerRef->Patrol();
}

// Called every frame
void AMeleeRat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeleeRat::LaunchPlayerUp()
{
	TSubclassOf<ACheesingCharacter> playerClass;
	TArray<AActor*> player;
	headRadius->GetOverlappingActors(player, playerClass);

	UE_LOG(LogTemp, Display, TEXT("Player Detected"));

	for (AActor* playerA : player)
	{
		if (ACheesingCharacter* cPlayer = Cast<ACheesingCharacter>(playerA))
		{
			cPlayer->LaunchCharacter(FVector(0, 0, upForce), false, true);
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald, TEXT("Launched"));
		}
	}
}

void AMeleeRat::TakeDamageM(int damage)
{
	health -= damage;

	UE_LOG(LogTemp, Warning, TEXT("Took Damage"));
	if (health <= 0)
	{
		Destroy(true);
	}
}
