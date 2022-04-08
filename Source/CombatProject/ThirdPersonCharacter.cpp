// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Weapon.h"
#include "ClassicEnemy.h"
#include "Animation/AnimInstance.h"
#include "Components/SphereComponent.h"


#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AThirdPersonCharacter::AThirdPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Spring Arm and attach it to the Root(CapsuleComponent)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 400.f; // Distance from player
	SpringArm->bUsePawnControlRotation = true; // Rotate arm based on controller

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	

	HealthCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HealthCollider"));
	HealthCollider->SetupAttachment(GetRootComponent());
	HealthCollider->SetGenerateOverlapEvents(true);

	AutoPossessPlayer = EAutoReceiveInput::Player0;


	// Allows the Character to stand still and face same direction 
	// Do not rotate when Controller does
	// That only affects the Camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	InterpSpeed = 15.f;
	bInterpToEnemy = false;

	// Set Default Animation Values

	// int
	runTimer = 0;
	pre_runTimer = 0;
	DebugMode = 0;


	// bool
	bisRun = false;
	bisWalk = false;
	bisHoldSpace = false;
	bisRoll = false;
	bisDodge = false;
	bIsForwardLock = false;
	bisAttack = false;
	bIsAttackSecondary = false;
	bisDebug = false;
	bisLocomotion = true;
	bisSnapRotation = false;
	bisThrow = false;

	// floats
	Speed_Run = 450.f;
	Speed_Jog = 350.f;
	Speed_Walk = 100.f;
	
}


// Called when the game starts or when spawned
void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();


	HealthCollider->OnComponentBeginOverlap.AddDynamic(this, &AThirdPersonCharacter::HealthColliderBeginOverlap);

	GetCharacterMovement()->MaxWalkSpeed = 330.f;

	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), "DisableAllScreenMessages");

}

// Called every frame
void AThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Show vectors when we want to debug
	if (bisDebug)
	{
		DebugVector();
	}



	// Fix Some Movement in Air
	if (GetCharacterMovement()->IsFalling())
	{
		
		GetCharacterMovement()->bOrientRotationToMovement = false;

	}
	else
	{

		GetCharacterMovement()->bOrientRotationToMovement = true;

	}



	// Check if SnapRotation is on - if so run function
	if (bisSnapRotation)
	{

		SnapRotationFn();

	}


	// Handle Movement Speed and Rotation Rate
	RunorWalk();


	InterpToEnemy(DeltaTime);


	if (Retrigger)
	{
		RetriggerClock += DeltaTime;

		if (RetriggerClock > RetriggerDelay)
		{
			Retrigger = false;
			bisSnapRotation = false;
			RetriggerClock = 0.f;
		}
	}


	if (Health == 0.f)
	{
		bDead = true;
	}


}



// Called to bind functionality to input
void AThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Moving with WASD
	PlayerInputComponent->BindAxis("MoveForward", this, &AThirdPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AThirdPersonCharacter::MoveRight);

	// Use
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AThirdPersonCharacter::Use);

	// Attack
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AThirdPersonCharacter::LeftMouseButtonPress);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AThirdPersonCharacter::LeftMouseButtonRelease);

	// Attack Secondary
	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &AThirdPersonCharacter::RightMouseButtonPress);
	PlayerInputComponent->BindAction("RMB", IE_Released, this, &AThirdPersonCharacter::RightMouseButtonRelease);

	// Sprint
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AThirdPersonCharacter::SprintPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AThirdPersonCharacter::SprintReleased);

	// Sprint
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AThirdPersonCharacter::Dodge);

	// Debug
	PlayerInputComponent->BindAction("Debug", IE_Pressed, this, &AThirdPersonCharacter::ChangeDebugMode);

	// ForwardLock
	PlayerInputComponent->BindAction("ForwardLock", IE_Pressed, this, &AThirdPersonCharacter::ForwardLock);

	// Turn Camera with mouse
	PlayerInputComponent->BindAxis("TurnCamera", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUpCamera", this, &APawn::AddControllerPitchInput);
}

/** Inputs */

void AThirdPersonCharacter::MoveForward(float Value)
{
	inputY = Value;

	FRotator ControlRotation = Controller->GetControlRotation();

	ControlRotation.Roll = 0.f;
	ControlRotation.Pitch = 0.f;

	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ControlRotation);
	FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRotation);

	ForwardVector *= inputY;
	RightVector *= inputX;

	if (!FMath::IsNearlyZero(Value))
	{
		AddMovementInput(ForwardVector);
	}
	// Set inputVector_World
	inputVector_World = ((ForwardVector * inputY) + (RightVector * inputX));


	// Set Local X and Y inputs
	FTransform Transform = GetActorTransform();
	FVector LocalXYVector = UKismetMathLibrary::InverseTransformDirection(Transform, inputVector_World);

	inputX_Local = LocalXYVector.Y;
	inputY_Local = LocalXYVector.X;

	
}

void AThirdPersonCharacter::MoveRight(float Value)
{
	inputX = Value;


	FRotator ControlRotation = Controller->GetControlRotation();

	ControlRotation.Roll = 0.f;
	ControlRotation.Pitch = 0.f;

	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ControlRotation);
	FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRotation);

	ForwardVector *= inputY;
	RightVector *= inputX;

	if (!FMath::IsNearlyZero(Value))
	{
		AddMovementInput(RightVector);
	}
	


	// Set inputVector_World
	inputVector_World = ((ForwardVector ) + (RightVector ));

	// Set Local X and Y inputs
	FTransform Transform = GetActorTransform();
	FVector LocalXYVector = UKismetMathLibrary::InverseTransformDirection(Transform, inputVector_World);

	inputX_Local = LocalXYVector.Y;
	inputY_Local = LocalXYVector.X;



}

void AThirdPersonCharacter::Use()
{

	if (ActiveOverlappingWeapon)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingWeapon);
		if (Weapon)
		{
			Weapon->Equip(this);
		}
	}

}

void AThirdPersonCharacter::ForwardLock()
{

	if (bIsForwardLock)
	{
		SetForwardLock(false);
		SetInterpToEnemy(false);
	}
	else
	{
		SetForwardLock(true);
		SetInterpToEnemy(true);
	}
}

void AThirdPersonCharacter::LeftMouseButtonPress()
{
	bisAttack = true;
}

void AThirdPersonCharacter::LeftMouseButtonRelease()
{
	bisAttack = false;
}

void AThirdPersonCharacter::RightMouseButtonPress()
{

	bIsAttackSecondary = true;

}

void AThirdPersonCharacter::RightMouseButtonRelease()
{

	bIsAttackSecondary = false;

}

void AThirdPersonCharacter::SprintPressed()
{

	bisHoldSpace = true;

}

void AThirdPersonCharacter::SprintReleased()
{

	bisHoldSpace = false;

	pre_runTimer = 0;

	if (runTimer < 10 && !bisDodge)
	{

		if (!bisRoll)
		{
			inputVector = FVector(inputX_Local, inputY_Local, 0.f);
		}

		LineEndTemp = GetEndPoint(100.f);

		if (GetCharacterMovement()->IsMovingOnGround())
		{
		
			bisRoll = true;
			UE_LOG(LogTemp, Warning, TEXT("State: Roll"));
			

		}
	}
	else
	{
		// for jump we might need to adjust this
		runTimer = 0;
	}

}

void AThirdPersonCharacter::Dodge()
{
	if (!bisRoll)
	{
		if (!bisDodge)
		{
			inputVector = FVector(inputX_Local, inputY_Local, 0.f);
		}

		LineEndTemp = GetEndPoint(100.f);

		if (GetCharacterMovement()->IsMovingOnGround())
		{

			bisDodge = true;
			UE_LOG(LogTemp, Warning, TEXT("State: Dodge"));


		}
	}
	


}

void AThirdPersonCharacter::ChangeDebugMode()
{
	switch (DebugMode)
	{
	case 0:
		bisDebug = true;
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), "DisableAllScreenMessages");
		DebugMode = 1;
		UE_LOG(LogTemp, Warning, TEXT("Debug Mode 1"));
		break;
	case 1:
		bisDebug = true;
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), "EnableAllScreenMessages");
		DebugMode = 2;
		UE_LOG(LogTemp, Warning, TEXT("Debug Mode 2"));
		break;
	case 2:
		bisDebug = false;
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), "DisableAllScreenMessages");
		DebugMode = 0;
		UE_LOG(LogTemp, Warning, TEXT("Debug Mode 0"));
		break;
	default:
		break;

	}

}

void AThirdPersonCharacter::HealthColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (!isInvisible)
	{
		AClassicEnemy* ClassicEnemy = Cast<AClassicEnemy>(OtherActor);
		if (ClassicEnemy)
		{
			if (OtherComponent == ClassicEnemy->RightHandCollider)
			{
				if (ClassicEnemy->isDamaging)
				{
					LoseHealth(ClassicEnemy->RightHandDamage);
				}
				
			}
			if (OtherComponent == ClassicEnemy->LeftHandCollider)
			{
				if (ClassicEnemy->isDamaging)
				{
					LoseHealth(ClassicEnemy->LeftHandDamage);
				}
			}
		}

	}

}

void AThirdPersonCharacter::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}



void AThirdPersonCharacter::SetForwardLock(bool input)
{

	if (input)
	{

		bIsForwardLock = true;
		UE_LOG(LogTemp, Warning, TEXT("ForwardLock: On"));
	}
	else
	{
		bIsForwardLock = false;
		UE_LOG(LogTemp, Warning, TEXT("ForwardLock: Off"));
	}
}

void AThirdPersonCharacter::InterpToEnemy(float Delta)
{
	

	if (bInterpToEnemy && CombatTarget && bisLocomotion)
	{
		SpringArm->bUsePawnControlRotation = false;
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CombatTarget->GetActorLocation());
		LookAtRotation.Pitch = -15.f;
		LookAtRotation.Roll = 0.f;

		SpringArm->SetWorldRotation(LookAtRotation);
		
		Controller->SetControlRotation(SpringArm->GetComponentRotation());
	}
	else
	{
		SpringArm->bUsePawnControlRotation = true;
	}


}





/** Debug Vector Setup */


void AThirdPersonCharacter::DebugVector()
{

	UWorld* World = GetWorld();

	if (World)
	{


		// Draw Forward Vector
		UKismetSystemLibrary::DrawDebugArrow(World, GetActorLocation(), GetActorLocation() - (GetActorForwardVector() * -150.f), 20.f, FLinearColor::Blue, 0.f, 2.5f);



		// Draw InputVector
		UKismetSystemLibrary::DrawDebugArrow(World, GetActorLocation(), GetActorLocation() - GetEndPoint(200.f), 20.f, FLinearColor::Red, 0.f, 5.f);



		// Temp Input Vector : Last "spacebar" direction input
		UKismetSystemLibrary::DrawDebugArrow(World, GetActorLocation(), GetActorLocation() - LineEndTemp, 20.f, FLinearColor::Green, 0.f, 5.f);



	}

}

FVector AThirdPersonCharacter::GetEndPoint(float input)
{

	FRotator ControlRotation = GetControlRotation();

	ControlRotation.Roll = 0.f;
	ControlRotation.Pitch = 0.f;




	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ControlRotation);
	ForwardVector *= inputY;


	FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRotation);
	RightVector *= inputX;




	FVector OutputVector = ForwardVector + RightVector;

	OutputVector.Normalize(0.0001f);
	OutputVector *= -input;


	return OutputVector;

}

void AThirdPersonCharacter::SnapRotationFn()
{

	FRotator ControlRotation = GetControlRotation();

	ControlRotation.Roll = 0.f;
	ControlRotation.Pitch = 0.f;

	float DeltaSeconds{};
	UWorld* World = GetWorld();
	if (World)
	{

		DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(World);

	}
	
	float Alpha = FMath::FInterpTo(0.f, 1.f, DeltaSeconds, 20.f);

	FRotator NewRotation = UKismetMathLibrary::RLerp(GetActorRotation(), ControlRotation, Alpha, true);

	SetActorRotation(NewRotation);

}

void AThirdPersonCharacter::RunorWalk()
{

	if (bisHoldSpace && bisLocomotion)
	{

		pre_runTimer++;



		if (pre_runTimer > 20)
		{


			bisRun = true;

			float MaxSpeedTemp = GetCharacterMovement()->MaxWalkSpeed;

	
			GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
			bIsForwardLock = false;

			float DeltaSeconds{};
			UWorld* World = GetWorld();
			if (World)
			{

				DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(World);

			}

			float ReturnValue = FMath::FInterpTo(MaxSpeedTemp, Speed_Run, DeltaSeconds, 10.f);


			GetCharacterMovement()->MaxWalkSpeed = ReturnValue;

			runTimer++;

			UE_LOG(LogTemp, Warning, TEXT("State: Running"));

		}

	}
	// This is if we Walk or Jog
	else
	{

		bisRun = false;

		if (bIsForwardLock)
		{

			//GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 0.f);

		}
		else
		{

			GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

		}


		if (bisWalk)
		{

			float MaxSpeedTemp = GetCharacterMovement()->MaxWalkSpeed;


			float DeltaSeconds{};
			UWorld* World = GetWorld();
			if (World)
			{

				DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(World);

			}

			float ReturnValue = FMath::FInterpTo(MaxSpeedTemp, Speed_Walk, DeltaSeconds, 10.f);


			GetCharacterMovement()->MaxWalkSpeed = ReturnValue;


		}
		else
		{
			float MaxSpeedTemp = GetCharacterMovement()->MaxWalkSpeed;


			float DeltaSeconds{};
			UWorld* World = GetWorld();
			if (World)
			{

				DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(World);

			}

			float ReturnValue = FMath::FInterpTo(MaxSpeedTemp, Speed_Jog, DeltaSeconds, 10.f);


			GetCharacterMovement()->MaxWalkSpeed = ReturnValue;


		}

	}
}




/** Set function for our EquippedWeapon */

void AThirdPersonCharacter::SetEquippedWeapon(AWeapon* WeaponToSet)
{

	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = WeaponToSet;


}


void AThirdPersonCharacter::ResetDodge()
{

	bisDodge = false;
	UE_LOG(LogTemp, Warning, TEXT("Reset Dodge"));
}

void AThirdPersonCharacter::ResetRoll()
{

	bisRoll = false;
	UE_LOG(LogTemp, Warning, TEXT("Reset Roll"));
}

void AThirdPersonCharacter::SnapRotation()
{

	bisSnapRotation = true;
	Retrigger = true;
	RetriggerClock = 0.f;

}

void AThirdPersonCharacter::ResetAttack()
{

	UE_LOG(LogTemp, Warning, TEXT("Reset Attack"));
	bisAttack = false;
	bIsAttackSecondary = false;

}

void AThirdPersonCharacter::LoseHealth(float Value)
{
	
	Health -= Value;
	if (Health < 0.f)
	{
		Health = 0.f;
	}
}
