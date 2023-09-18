// Copyright Epic Games, Inc. All Rights Reserved.

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


void ACheesingCharacter::Roll()
{
	//FVector velocity = GetVelocity();
	//UE_LOG(LogTemp, Display, TEXT("Teste2"));
	//UE_LOG(LogTemp, Display, TEXT("Velocity: %f"), velocity.Y);
	
	if (stateEnum == ECharstate::VE_Walking)
	{
		stateEnum = ECharstate::VE_Rolling;
		UE_LOG(LogTemp, Display, TEXT("Rolling"));

		moveComponent->MaxWalkSpeed = rollSpeed;
		moveComponent->MaxAcceleration = rollAcceleration;
	}
	else if (stateEnum == ECharstate::VE_Rolling)
	{
		stateEnum = ECharstate::VE_Walking;
		UE_LOG(LogTemp, Display, TEXT("Walking"));

		moveComponent->MaxWalkSpeed = normalWalkSpeed;
		moveComponent->MaxAcceleration = normalAcceleration;
	}
	
	
}

void ACheesingCharacter::MeleeAttack()
{
	UE_LOG(LogTemp, Display, TEXT("Cooldown: %f"), normalCooldown);
	if (normalCooldown <= 0)
	{
		stateEnum = ECharstate::VE_Attacking;
		UE_LOG(LogTemp, Display, TEXT("Attacking"));

		//Inserir a l�gica de ataque  	
		attackRadiusTeste->GetOverlappingActors(overlapingActors);

		for (AActor* actor : overlapingActors)
		{
			FString s;
			actor->GetName(s);

			UE_LOG(LogTemp, Display, TEXT("Actor: %s"), *s);

			if (IDamageInterface* dActor = Cast<IDamageInterface>(actor))
			{
				dActor->TakeDamageM(10); //Casta a interface no objeto q implementou ela
			}
		}

		stateEnum = ECharstate::VE_Walking;

		normalCooldown = attackCooldown;
	}
}

void ACheesingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (normalCooldown > 0)
	{
		normalCooldown -= DeltaTime;
	}
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
