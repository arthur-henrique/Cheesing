// Fill out your copyright notice in the Description page of Project Settings.


#include "Trampoline.h"

// Sets default values
ATrampoline::ATrampoline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrampoline::BeginPlay()
{
	Super::BeginPlay();
	
	gameInstance = Cast<UCheesingGameInstance>(GetGameInstance());

}

// Called every frame
void ATrampoline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrampoline::LaunchPlayerUp()
{
	

	TSubclassOf<ACheesingCharacter> playerClass;
	TArray<AActor*> player;
	launchRadius->GetOverlappingActors(player, playerClass);

	//UE_LOG(LogTemp, Display, TEXT("Player Detected"));

	for (AActor* playerA : player)
	{
		if (ACheesingCharacter* cPlayer = Cast<ACheesingCharacter>(playerA))
		{
			if (cPlayer->CanJump())
			{
			cPlayer->LaunchCharacter(FVector(0, 0, upForce), false, true);
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald, TEXT("Launched"));
			}
			
		}
	}
}