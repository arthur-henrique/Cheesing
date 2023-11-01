// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/CharacterMovementComponent.h" 
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Runtime/Engine/Classes/Components/SphereComponent.h>
#include <DrawDebugHelpers.h>
#include <Projectile.h>
#include "InteractionInterface.h"

#include "CheesingCharacter.generated.h"


UENUM(BlueprintType)
enum class ECharstate : uint8
{
	VE_Walking UMETA(DisplayName = "Walking"),
	VE_Rolling UMETA(DisplayName = "Rolling"),
	VE_Attacking UMETA(DisplayName = "Attacking"),
	VE_Dashing UMETA(DisplayName = "Dashing"),
	VE_Aiming UMETA(DisplayName = "Aiming"),

};


UCLASS(config=Game)
class ACheesingCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ACheesingCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int health;

protected:
	UFUNCTION(BlueprintCallable)
	void Roll();

	UFUNCTION(BlueprintCallable)
	void MeleeAttack();

	UFUNCTION(BlueprintCallable)
	void Dash();

	void DashCooldown();

	UFUNCTION()
		void Landed(const FHitResult& Hit) override;


	UFUNCTION(blueprintCallable)
	void InteractTo();

	float GetFloatVelocity();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roll")
	float rollSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roll")
	float rollAcceleration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float dashSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float dashCooldown;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dash")
	bool canDash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float attackCooldown;

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	USphereComponent* attackRadiusTeste;

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	UStaticMeshComponent* projectilePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	bool hasAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<AProjectile> projectile;

	TArray<AActor*> overlapingActors;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool debugMode;

	FTimerHandle dashTimerHandle;

private:
	float normalWalkSpeed;

	float normalAcceleration;

	float normalCooldown;

	float timeDelta;

	bool changingCamera;

	float originalGravity;

	UCharacterMovementComponent* moveComponent;

	
public:

	UFUNCTION(BlueprintCallable)
	void Aim();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void RechargeAmmo();

	UFUNCTION(BlueprintCallable, Category = "Damage")
	bool TookDamage(int damage);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ECharstate stateEnum;

};

