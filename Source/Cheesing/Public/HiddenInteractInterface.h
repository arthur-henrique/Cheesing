// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HiddenInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI)
class UHiddenInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CHEESING_API IHiddenInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Activation")
	void OnActivation();
	virtual void OnActivation_Implementation();
};
