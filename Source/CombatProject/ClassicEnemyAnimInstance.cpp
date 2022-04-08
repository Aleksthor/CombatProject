// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicEnemyAnimInstance.h"
#include "ClassicEnemy.h"

void UClassicEnemyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			ClassicEnemy = Cast<AClassicEnemy>(Pawn);
		}
	}


}

void UClassicEnemyAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	
	}
	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();


		ClassicEnemy = Cast<AClassicEnemy>(Pawn);
	}

}
