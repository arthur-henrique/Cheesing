// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheesingGameInstance.h"
#include "InteractionInterface.h"

#include "Collectible.generated.h"

UCLASS()
class CHEESING_API ACollectible : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectible();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere,  Category = "Collectible Properties")
	int id;

	UPROPERTY(BlueprintReadWrite, Category = "Collectible Properties")
	bool wasCollected;

};
