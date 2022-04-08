// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "PlayerCharacter.h"


void UCharacterAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner(); // Inherited from UAnimInstance
	}


}

void UCharacterAnimInstance::UpdateAnimationProperties()
{

	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner(); // Inherited from UAnimInstance
	}
	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		//RightVector = Cast<APlayerCharacter>(Pawn)->RightInputValue;
	}

}
