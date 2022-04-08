// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "ThirdPersonCharacter.h"
#include "ClassicEnemy.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupCollider = CreateDefaultSubobject<USphereComponent>(TEXT("PickupColldier"));
	SetRootComponent(PickupCollider);



	AttackCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackColldier"));
	AttackCollider->SetupAttachment(GetRootComponent());

	bEquipSetVisibility = false;


	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());

	WeaponState = EWeaponState::EWS_Pickup;
}



// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	PickupCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapPickup);
	PickupCollider->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnOverlapEndPickup);

	AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapAttack);
	AttackCollider->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnOverlapEndAttack);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	if (isOverlapping)
	{
		if (TimesGivenDamage == 0)
		{
			AThirdPersonCharacter* ThirdPersonCharacter = Cast<AThirdPersonCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
			if (ThirdPersonCharacter->isDamaging)
			{
				UE_LOG(LogTemp, Warning, TEXT("HIT"));
				if (ClassicEnemy)
				{
					ClassicEnemy->Health -= WeaponDamage;
					if (ClassicEnemy->Health < 0.f)
					{
						ClassicEnemy->Health = 0.f;
					}
				}
			}
			TimesGivenDamage++;
		}
	}

}

void AWeapon::OnOverlapPickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor)
	{
		AThirdPersonCharacter* Player = Cast<AThirdPersonCharacter>(OtherActor);
		if (Player && !Equipped)
		{
			bEquipSetVisibility = true;
			Player->SetActiveOverlappingItem(this);
		}
		APlayerCharacter* Player2 = Cast<APlayerCharacter>(OtherActor);
		if (Player2 && !Equipped)
		{
			bEquipSetVisibility = true;
			Player2->SetActiveOverlappingItem(this);
		}
	}
}

void AWeapon::OnOverlapEndPickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{

	if (OtherActor)
	{
		AThirdPersonCharacter* Player = Cast<AThirdPersonCharacter>(OtherActor);
		if (Player )
		{
			bEquipSetVisibility = false;
			Player->SetActiveOverlappingItem(nullptr);
		}
		APlayerCharacter* Player2 = Cast<APlayerCharacter>(OtherActor);
		if (Player2 )
		{
			bEquipSetVisibility = false;
			Player2->SetActiveOverlappingItem(nullptr);
		}
	}
}

void AWeapon::OnOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ClassicEnemy = Cast<AClassicEnemy>(OtherActor);
	if (ClassicEnemy)
	{
		if (OtherComponent == ClassicEnemy->GetRootComponent())
		{
			isOverlapping = true;
		}
	
	}
}

void AWeapon::OnOverlapEndAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	
	ClassicEnemy = Cast<AClassicEnemy>(OtherActor);
	if (ClassicEnemy)
	{
		if (OtherComponent == ClassicEnemy->GetRootComponent())
		{
			isOverlapping = true;
			TimesGivenDamage = 0;
		}

	}

}

// Attaching item to a Socket
void AWeapon::Equip(ACharacter* Char)
{
	if (Char)
	{

		AThirdPersonCharacter* Player = Cast<AThirdPersonCharacter>(Char);
		if (Player)
		{
			Equipped = true;

			WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
			WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
			WeaponMesh->SetSimulatePhysics(false);

			PickupCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
			PickupCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

			const USkeletalMeshSocket* RightHandSocket = Player->GetMesh()->GetSocketByName("RightHandSocket");
			if (RightHandSocket)
			{
				RightHandSocket->AttachActor(this, Player->GetMesh());

			
				Player->SetActiveOverlappingItem(nullptr);
				Player->SetEquippedWeapon(this);

			}
			if (OnEquipSound)
			{
			UGameplayStatics::PlaySound2D(this, OnEquipSound);
			}
		}
		APlayerCharacter* Player2 = Cast<APlayerCharacter>(Char);
		if (Player2)
		{
			Equipped = true;

			WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
			WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
			WeaponMesh->SetSimulatePhysics(false);

			PickupCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
			PickupCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

			const USkeletalMeshSocket* RightHandSocket = Player2->GetMesh()->GetSocketByName("RightHandSocket");
			if (RightHandSocket)
			{
				RightHandSocket->AttachActor(this, Player2->GetMesh());


				Player2->SetActiveOverlappingItem(nullptr);
				Player2->SetEquippedWeapon(this);

			}
			if (OnEquipSound)
			{
				UGameplayStatics::PlaySound2D(this, OnEquipSound);
			}
		}

	}
}
