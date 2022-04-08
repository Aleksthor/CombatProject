// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Weapon.h"
#include "Animation/AnimInstance.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleSize(30.f, 90.f);

	// Create Spring Arm and attach it to the Root(CapsuleComponent)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 400.f; // Distance from player
	SpringArm->bUsePawnControlRotation = true; // Rotate arm based on controller

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// So character stand still with no movement
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	RightInputValue = 0.f;
	JumpVelocity = 700.f;
	bLMBDown = false;

	bAttacking = false;


	// Configure Character Movement.
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character Moves in the Direction of Input...
	GetCharacterMovement()->RotationRate = FRotator(0.f, 700.f, 0.f); // ... at this rotation rate / Smooth transitions between directions
	GetCharacterMovement()->JumpZVelocity = JumpVelocity; // Jump Velocity
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->MaxWalkSpeed = 330.f;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = 330.f;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Moving with WASD
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// Turn Camera with mouse
	PlayerInputComponent->BindAxis("TurnCamera", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUpCamera", this, &APawn::AddControllerPitchInput);

	// Sprint
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprint);

	// Left Mouse Button
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &APlayerCharacter::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &APlayerCharacter::LMBUp);

	// Use
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &APlayerCharacter::Use);
	
}

void APlayerCharacter::MoveForward(float Value)
{
	// Ensures no crashing checking if its not a nullptr
	if ((Controller != nullptr) && (Value != 0.f) && (!bAttacking))
	{
		// Find out what way is forward
		// Getting the Vector of the Controller. This is our SpringArm
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); // Canceling out X and Z Rotation.

		// Here we are basically getting the local X vector with a unit of 1
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	// Ensures no crashing checking if its not a nullptr
	if ((Controller != nullptr) && (Value != 0.f) && (!bAttacking))
	{
		// Find out what way is forward
		// Getting the Vector of the Controller. This is our SpringArm
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); // Canceling out X and Z Rotation.

		// Here we are basically getting the local Y vector with a unit of 1
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);

		RightInputValue = Value;
	}
}

void APlayerCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 450.f;
}

void APlayerCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 330.f;
}

void APlayerCharacter::LMBDown()
{
	if (EquippedWeapon)
	{
		Attack();
	}
}

void APlayerCharacter::LMBUp()
{	
	bLMBDown = false;
}

void APlayerCharacter::Attack()
{
	if (!bAttacking)
	{
		
		bAttacking = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			if (CombatMontage)
			{
			
				AnimInstance->Montage_Play(CombatMontage, 1.5f);
				AnimInstance->Montage_JumpToSection(FName("Attack1"), CombatMontage);
				
			}
		
		}
	}

}

void APlayerCharacter::AttackEnd()
{
	bAttacking = false;
	if (bLMBDown)
	{
		Attack();
		
	}
}

void APlayerCharacter::Use()
{
	bLMBDown = true;

	if (ActiveOverlappingWeapon)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingWeapon);
		if(Weapon)
		{
			Weapon->Equip(this);
		}
	}
}

void APlayerCharacter::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = WeaponToSet;
}


