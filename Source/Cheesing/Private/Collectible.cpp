// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible.h"

// Sets default values
ACollectible::ACollectible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

void ACollectible::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* gameInstance = GetGameInstance();
	

	if (UCheesingGameInstance* game = Cast<UCheesingGameInstance>(gameInstance))
	{		
		game->AddToCollectiblesArray(this);
	}
}
