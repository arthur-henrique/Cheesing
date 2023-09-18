// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DamageInterface.h"

#include "BasicEnemy.generated.h"

UCLASS()
class CHEESING_API ABasicEnemy : public APawn, public IDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasicEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Damage Interface
	virtual void TakeDamageM(int damage) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Health")
	int health;

};
