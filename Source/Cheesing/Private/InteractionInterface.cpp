// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionInterface.h"

// Add default functionality here for any IInteractionInterface functions that are not pure virtual.

void IInteractionInterface::Interact()
{
#if WITH_EDITOR
	UE_LOG(LogTemp, Display, TEXT("Interagiu"));
#endif // WITH_EDITOR

}
