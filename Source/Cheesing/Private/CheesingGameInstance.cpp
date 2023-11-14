/**
 * @file CheesingGameInstance.cpp
 * @author Gabriel Tury 
 * @brief Implementation of the CheesingGameInstance class.
 *
 * This file contains the implementation of the CheesingGameInstance class, which is part of the Unreal Engine project.
 */

// Fill out your copyright notice in the Description page of Project Settings.
#include "CheesingGameInstance.h"

/**
 * @brief Get the ID of a collectible from the specified index.
 *
 * This function retrieves the ID of a collectible from the specified index in the collectiblesArray.
 *
 * @param index The index of the collectible.
 * @return The ID of the collectible.
 */
int UCheesingGameInstance::GetIdFromIndex(int index)
{
	ACollectible* item;
	collectiblesArray.Find(item, index);

	return item->id;
}

/**
 * @brief Get the collection state of a collectible from the specified index.
 *
 * This function retrieves the collection state of a collectible from the specified index in the collectiblesArray.
 *
 * @param index The index of the collectible.
 * @return True if the collectible was collected, false otherwise.
 */
bool UCheesingGameInstance::GetCollectStateFromIndex(int index)
{
	ACollectible* item;
	collectiblesArray.Find(item, index);

	return item -> wasCollected;
}

/**
 * @brief Add a collectible to the collectiblesArray.
 *
 * This function adds a collectible to the collectiblesArray for tracking purposes.
 *
 * @param collectibleToAdd The collectible to add to the array.
 */

void UCheesingGameInstance::AddToCollectiblesArray(ACollectible* collectibleToAdd)
{
	collectiblesArray.Add(collectibleToAdd);
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Add");
	//UE_LOG(LogTemp, Display, TEXT("Adicionou"));
}

FVector UCheesingGameInstance::GetCheckpoint()
{
	if (!lastCheckpoint.IsZero())
		return lastCheckpoint;
	else
		return FVector::ZeroVector;
}

void UCheesingGameInstance::ReturnToCheckpoint()
{
	playerRef->SetActorLocation(lastCheckpoint);
}
