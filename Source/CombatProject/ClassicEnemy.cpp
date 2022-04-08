// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicEnemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "ThirdPersonCharacter.h"
#include "AIHandsHitBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
AClassicEnemy::AClassicEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	AgroCollider = CreateDefaultSubobject<USphereComponent>(TEXT("AgroCollider"));
	AgroCollider->SetupAttachment(GetRootComponent());
	AgroCollider->InitSphereRadius(1000.f);

	CombatCollider = CreateDefaultSubobject<USphereComponent>(TEXT("CombatCollider"));
	CombatCollider->SetupAttachment(GetRootComponent());
	CombatCollider->InitSphereRadius(100.f);


	RightHandCollider = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandCollider"));
	RightHandCollider->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
	
	LeftHandCollider = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandCollider"));
	LeftHandCollider->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AClassicEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());

	AgroCollider->OnComponentBeginOverlap.AddDynamic(this, &AClassicEnemy::AgroColldierBeginOverlap);
	AgroCollider->OnComponentEndOverlap.AddDynamic(this, &AClassicEnemy::AgroColldierEndOverlap);

	CombatCollider->OnComponentBeginOverlap.AddDynamic(this, &AClassicEnemy::CombatColliderBeginOverlap);
	CombatCollider->OnComponentEndOverlap.AddDynamic(this, &AClassicEnemy::CombatColliderEndOverlap);


}

// Called every frame
void AClassicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AClassicEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AClassicEnemy::AgroColldierBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor)
	{
		AThirdPersonCharacter* ThirdPersonCharacter = Cast<AThirdPersonCharacter>(OtherActor);
		if (ThirdPersonCharacter)
		{
			ThirdPersonCharacter->SetCombatTarget(this);
			MoveToTarget(ThirdPersonCharacter);
		}
	}
}

void AClassicEnemy::AgroColldierEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	
	AThirdPersonCharacter* ThirdPersonCharacter = Cast<AThirdPersonCharacter>(OtherActor);
	if (ThirdPersonCharacter)
	{
		if (OtherComponent == ThirdPersonCharacter->GetRootComponent())
		{
			ThirdPersonCharacter->SetCombatTarget(nullptr);
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			if (AIController)
			{
				AIController->StopMovement();
			}
		}
	}
	
}

void AClassicEnemy::CombatColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AThirdPersonCharacter* ThirdPersonCharacter = Cast<AThirdPersonCharacter>(OtherActor);
		if (ThirdPersonCharacter)
		{
			CombatTarget = ThirdPersonCharacter;
			bOverlappingCombatSphere = true;
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
	}
}

void AClassicEnemy::CombatColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AThirdPersonCharacter* ThirdPersonCharacter = Cast<AThirdPersonCharacter>(OtherActor);
		if (ThirdPersonCharacter)
		{
			bOverlappingCombatSphere = false;
			if (EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking)
			{
				MoveToTarget(ThirdPersonCharacter);
			}
		}
	}
}

void AClassicEnemy::MoveToTarget(AThirdPersonCharacter* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		if (LongerAcceptanceRadius)
		{
			MoveRequest.SetAcceptanceRadius(100.f);
		}
		else
		{
			MoveRequest.SetAcceptanceRadius(5.f);
		}
		// Struct for NavPath Info
		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);

		/**
		TArray<FNavPathPoint> PathPoints = NavPath->GetPathPoints();
		for (auto Point : PathPoints)
		{
			FVector Location = Point.Location;
			UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 10.f, 0.5f);
		}
		*/
	}
}

