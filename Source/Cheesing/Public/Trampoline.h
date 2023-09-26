// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Cheesing/CheesingCharacter.h>
#include <Components/SphereComponent.h>

#include "Trampoline.generated.h"

UCLASS()
class CHEESING_API ATrampoline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrampoline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void LaunchPlayerUp();

	UPROPERTY(BlueprintReadWrite)
	USphereComponent* launchRadius;

	UPROPERTY(EditAnywhere, Category = "Launch")
	float upForce;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
