// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Collectible.h"
#include <Cheesing/CheesingCharacter.h>

#include "CheesingGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CHEESING_API UCheesingGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Collectibles")
	TArray<ACollectible*> collectiblesArray;

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	ACheesingCharacter* playerRef;

	UPROPERTY(BlueprintReadWrite, Category = "Checkpoint")
	FVector lastCheckpoint;

protected:

public:
	UFUNCTION(BlueprintCallable, Category = "Collectibles")
	int GetIdFromIndex(int index);

	UFUNCTION(BlueprintCallable, Category = "Collectibles")
	bool GetCollectStateFromIndex(int index);

	UFUNCTION(BlueprintCallable, Category = "Collectibles")
		void AddToCollectiblesArray(ACollectible* collectibleToAdd);

	UFUNCTION(BlueprintCallable, Category = "Checkpoint")
	FVector GetCheckpoint();

	UFUNCTION(BlueprintCallable, Category = "Checkpoint")
	void ReturnToCheckpoint();

	
	
};
