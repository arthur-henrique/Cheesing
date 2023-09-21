// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicEnemy.h"
#include <Cheesing/CheesingCharacter.h>

// Sets default values
ABasicEnemy::ABasicEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasicEnemy::LaunchPlayerUp()
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


// Called every frame
void ABasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasicEnemy::TakeDamageM(int damage)
{
	health -= damage;

	UE_LOG(LogTemp, Warning, TEXT("Took Damage"));
	if (health <= 0)
	{
		Destroy(true);
	}
}

