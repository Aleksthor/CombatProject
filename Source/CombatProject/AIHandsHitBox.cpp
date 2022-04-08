// Fill out your copyright notice in the Description page of Project Settings.


#include "AIHandsHitBox.h"
#include "ThirdPersonCharacter.h"
#include "ClassicEnemy.h"
#include "Components/SphereComponent.h"

// Sets default values
AAIHandsHitBox::AAIHandsHitBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageCollider = CreateDefaultSubobject<USphereComponent>(TEXT("DamageCollider"));
	SetRootComponent(DamageCollider);
	DamageCollider->InitSphereRadius(20.f);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());

	Damage = 10.f;

}

// Called when the game starts or when spawned
void AAIHandsHitBox::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AAIHandsHitBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


