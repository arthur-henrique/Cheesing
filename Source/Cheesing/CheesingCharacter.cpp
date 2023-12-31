/*!
 *  @file CheesingCharacter.cpp
 *  @author Gabriel Tury
 *  @date 2023-10-16
 *  @brief Implementation of the CheesingCharacter class.
 *  @project Cheesing
 *
 *  Implements the cheesing character.
 */// Copyright Epic Games, Inc. All Rights Reserve

#include "CheesingCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include <Cheesing/Public/DamageInterface.h>
#include <Cheesing/Public/BasicEnemy.h>
#include <HiddenInteractInterface.h>



//////////////////////////////////////////////////////////////////////////
// ACheesingCharacter

ACheesingCharacter::ACheesingCharacter()
{
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm



	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


	moveComponent = GetCharacterMovement();
	normalWalkSpeed = moveComponent->MaxWalkSpeed;
	normalAcceleration = moveComponent->MaxAcceleration;
	originalGravity = moveComponent->GravityScale;
	stateEnum = ECharstate::VE_Walking;

	attackCooldown = 5.f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACheesingCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACheesingCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACheesingCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACheesingCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACheesingCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACheesingCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACheesingCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACheesingCharacter::OnResetVR);
}
/**Called to Recharge Character's ammo*/
void ACheesingCharacter::RechargeAmmo()
{
	hasAmmo = true;
}

/**Returns false if is dead and true if is still alive*/
bool ACheesingCharacter::TookDamage(int damage, FVector damagePosition)
{
	//What happens when receiving damage
	FVector damageDirection = damagePosition - GetActorLocation();
	
	damageDirection = FollowCamera->GetComponentLocation() - GetActorLocation();

	LaunchCharacter(damageDirection * knockbackPlayerMultiplier,true,true);

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Emerald, TEXT("Launched"));

	//moveComponent->AddImpulse(damageDirection * knockbackPlayerMultiplier);

	health-= damage;
	TookDmg();
	if (health <= 0)
	{
		isDead = true;
		OnDeath();
		return false;
		
	}
	else
	{
		return true;
	}
}

void ACheesingCharacter::ChangeSpeed(float speed)
{
	moveComponent->MaxWalkSpeed = speed;
}

void ACheesingCharacter::Bounced_Implementation()
{
	isBouncing = true;
}

/**Called to change character to rolling State*/
void ACheesingCharacter::Roll()
{
	//FVector velocity = GetVelocity();
	//UE_LOG(LogTemp, Display, TEXT("Teste2"));
	//UE_LOG(LogTemp, Display, TEXT("Velocity: %f"), velocity.Y);
	
	if (stateEnum == ECharstate::VE_Walking)
	{
		stateEnum = ECharstate::VE_Rolling;
		if(debugMode)
		UE_LOG(LogTemp, Display, TEXT("Rolling"));

		moveComponent->MaxWalkSpeed = rollSpeed;
		moveComponent->MaxAcceleration = rollAcceleration;
	}
	else if (stateEnum == ECharstate::VE_Rolling)
	{
		stateEnum = ECharstate::VE_Walking;
		if(debugMode)
		UE_LOG(LogTemp, Display, TEXT("Walking"));

		moveComponent->MaxWalkSpeed = normalWalkSpeed;
		moveComponent->MaxAcceleration = normalAcceleration;
	}
	
	
}

/**Called when M1 is pressed, handles attack in every state where it is possible */
void ACheesingCharacter::MeleeAttack()
{
	if (stateEnum == ECharstate::VE_Walking)
	{
		if(debugMode)
		UE_LOG(LogTemp, Display, TEXT("Cooldown: %f"), normalCooldown);

		if (normalCooldown <= 0)
		{
			stateEnum = ECharstate::VE_Attacking;
			if(debugMode)
			UE_LOG(LogTemp, Display, TEXT("Attacking"));

			//Inserir a l�gica de ataque  	
			attackRadiusTeste->GetOverlappingActors(overlapingActors);

			for (AActor* actor : overlapingActors)
			{
				FString s;
				actor->GetName(s);

				if(debugMode)
				UE_LOG(LogTemp, Display, TEXT("Actor: %s"), *s);

				if (IDamageInterface* dActor = Cast<IDamageInterface>(actor))
				{
					dActor->TakeDamageM(10); //Casta a interface no objeto q implementou ela
				}
				else if (actor->Implements<UHiddenInteractInterface>()) //Maneira Correta de implementar interfaces de bp e c++
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "WTF");
					IHiddenInteractInterface::Execute_OnActivation(actor);
				}
			}

			//Temporario para tirar o bug do AIM
			FTimerHandle walkingTimer;
			GetWorldTimerManager().SetTimer(walkingTimer,this, &ACheesingCharacter::SetCharWalking, .2f, false);
			

			normalCooldown = attackCooldown;
		}
	}
	else if (stateEnum == ECharstate::VE_Rolling && FMath::IsNearlyEqual(GetFloatVelocity(), rollSpeed, .1f))
	{
		Dash();
	}
	else if (stateEnum == ECharstate::VE_Aiming && hasAmmo)
	{

		hasAmmo = false;
		FHitResult hit;

		GetWorld()->LineTraceSingleByChannel(hit, FollowCamera->GetComponentLocation(), FollowCamera->GetForwardVector() * 5000, ECC_WorldDynamic);

		GetWorld()->SpawnActor<AProjectile>(projectile, projectilePoint->GetComponentLocation(), FollowCamera->GetComponentRotation());

		Aim();
#if WITH_EDITOR
		if (debugMode)
		{
			DrawDebugLine(GetWorld(), FollowCamera->GetComponentLocation(), FollowCamera->GetForwardVector() * 5000, FColor::Red, true, 60.f, false, 4.f);
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Emerald, TEXT("Shoot"));
			//UE_LOG(LogTemp, Display, TEXT("%s"), *hit.ImpactPoint.ToString()); Se habilitado esses logs, crasham a Unreal quando hit for nulo
//UE_LOG(LogTemp,Display,TEXT("%s"), *hit.GetActor()->GetName())	

		}
#endif //Debugs
		
	
	}
}
/**Called to make the character dash */
void ACheesingCharacter::Dash()
{
	if (canDash)
	{
		FVector dashVelocity = FVector(GetVelocity().X, GetVelocity().Y, 0.f);
		LaunchCharacter(dashVelocity * dashSpeedMultiplier, true, true);
		//Lanca  o persongame a frente multiplicando sua velocidade, se isso n�o for setado de volta para o padr�o, a velocidade sera a mesma at� tocar o ch�o (n�o tem maxAirSpeed)
		
		canDash = false;
		GetWorld()->GetTimerManager().SetTimer(dashTimerHandle, this, &ACheesingCharacter::DashCooldown, dashCooldown, false);

		
	}


	if(debugMode)
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald, TEXT("Dash"));
}

void ACheesingCharacter::SetCheckpointPosition()
{
	if (CanJump())
	{
		lastValidPosition = GetActorLocation();
	}
		

}

/**Called after dashcooldown(seconds)*/
void ACheesingCharacter::DashCooldown()
{
	GetWorld()->GetTimerManager().ClearTimer(dashTimerHandle);
	canDash = true;
}

void ACheesingCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, "Landed");
	moveComponent->GravityScale = 2.85f;
}

void ACheesingCharacter::SetCharWalking()
{
	stateEnum = ECharstate::VE_Walking;
}

/**Called to change camera to aiming State*/
void ACheesingCharacter::Aim()
{
	if (stateEnum == ECharstate::VE_Walking)
	{
		stateEnum = ECharstate::VE_Aiming;
		bUseControllerRotationYaw = true;
		changingCamera = true;
		
	}
	else if (stateEnum == ECharstate::VE_Aiming)
	{
		stateEnum = ECharstate::VE_Walking;
		bUseControllerRotationYaw = false;
		
		changingCamera = true;
	}

}

/**Checks for objetcs that implements Interaction Interface inside a radius*/
void ACheesingCharacter::InteractTo()
{
	attackRadiusTeste->GetOverlappingActors(overlapingActors);

	for (AActor* actor : overlapingActors)
	{
		

		if (debugMode)
		{
			FString s;
			actor->GetName(s);

			UE_LOG(LogTemp, Display, TEXT("Actor: %s"), *s);
		}
			
		if (actor->Implements<UInteractionInterface>())
		{
			IInteractionInterface::Execute_Interact(actor);
		}
		//if (IInteractionInterface* dActor = Cast<IInteractionInterface>(actor))
		//{
		//	dActor->Interact();
		//}
	}
}

/**Get character Velocity as a float( X, Y axis only)*/
float ACheesingCharacter::GetFloatVelocity()
{
	FVector velocityXY = FVector(GetVelocity().X, GetVelocity().Y, 0.f);
	return FMath::Sqrt(FMath::Pow(velocityXY.X, 2) + FMath::Pow(velocityXY.Y, 2));
}

void ACheesingCharacter::BeginPlay()
{
	Super::BeginPlay();
	stateEnum = ECharstate::VE_Walking;

	GetWorld()->GetTimerManager().SetTimer(checkpointTimerHandle, this, &ACheesingCharacter::SetCheckpointPosition, 1.f, true, 1.f);

	canDash = true;
}

void ACheesingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	if (changingCamera)
	{
		timeDelta += DeltaTime;

		if (stateEnum == ECharstate::VE_Aiming)
		{
			FollowCamera->SetRelativeLocation(FMath::Lerp(FVector(-115.f, -10.f, 106.f), FVector(50.f, 75.f, 90.f), timeDelta));
		}
		else if (stateEnum == ECharstate::VE_Walking)
		{
			FollowCamera->SetRelativeLocation(FMath::Lerp(FVector(50.f, 75.f, 90.f), FVector(-115.f, -10.f, 106.f), timeDelta));
		}
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, FString::Printf(TEXT("%f"), timeDelta));
		if (FMath::IsNearlyEqual(timeDelta,1,.1f) )
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Stop"));
			changingCamera = false;
			timeDelta = 0;
		}
	}
	
	if (normalCooldown > 0)
	{
		normalCooldown -= DeltaTime;
	}
	//else
	//{
	//	
	//	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Virou Walking");
	//}
}


void ACheesingCharacter::OnResetVR()
{
	// If Cheesing is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in Cheesing.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ACheesingCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ACheesingCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ACheesingCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACheesingCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACheesingCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACheesingCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
