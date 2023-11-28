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

void AMeleeRat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if(timer<2)
	//timer += DeltaTime;
}

/**Called when AI reaches its waypoint*/
void AMeleeRat::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!playerDetected && !dead /*&& timer > 1.f */)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, "Chamou Patrulha");
		controllerRef->Patrol();

		//timer = 0;
	}
	
}

void AMeleeRat::Die()
{
	SetActorHiddenInGame(true);
	//Destroy(true);
}
/**Launch Player upwards when enters a trigger*/
void AMeleeRat::LaunchPlayerUp()
{
	TSubclassOf<ACheesingCharacter> playerClass;
	TArray<AActor*> player;
	headRadius->GetOverlappingActors(player, playerClass);

	//UE_LOG(LogTemp, Display, TEXT("Player Detected"));

	for (AActor* playerA : player)
	{
		if (ACheesingCharacter* cPlayer = Cast<ACheesingCharacter>(playerA))
		{
			TakeDamageM(10);
			cPlayer->LaunchCharacter(FVector(0, 0, upForce), false, true);
			//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald, TEXT("Launched"));
		}
	}
}

void AMeleeRat::TakeDamageM(int damage)
{
	health -= damage;
	ReceivedDamage();
	//UE_LOG(LogTemp, Warning, TEXT("Took Damage"));
	if (health <= 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
		dead = true;
	}
}
/**Called to move player to the next waypoint*/
void AMeleeRat::MoveToPlayer()
{
	controllerRef->MoveToLocation(playerRef->GetActorLocation(), stoppingDistance, true);
}
/**Called when player is in range to follow its location*/
void AMeleeRat::SeekPlayer()
{
	if (!dead)
	{
		MoveToPlayer();
		GetWorld()->GetTimerManager().SetTimer(seekPlayerTimerHandle, this, &AMeleeRat::SeekPlayer, .25f, true);
	}
	
}
/**Called when player steps out of range*/
void AMeleeRat::StopSeekingPlayer()
{
	if(!dead)
	GetWorld()->GetTimerManager().ClearTimer(seekPlayerTimerHandle);
	//controllerRef->Patrol();
}

void AMeleeRat::Attacking_Implementation()
{

}
