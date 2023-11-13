// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageInterface.h"

#include "Boss.generated.h"

UCLASS()
class CHEESING_API ABoss : public AActor, public IDamageInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void TakeDamageM(int damage) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
