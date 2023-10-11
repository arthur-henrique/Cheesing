// Fill out your copyright notice in the Description page of Project Settings.


#include "HiddenInteractInterface.h"

// Add default functionality here for any IHiddenInteractInterface functions that are not pure virtual.

void IHiddenInteractInterface::OnActivation_Implementation()
{
#if WITH_EDITOR
	UE_LOG(LogTemp, Display, TEXT("Ativou"));
#endif
}
