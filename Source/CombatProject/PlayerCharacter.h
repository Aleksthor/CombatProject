// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class COMBATPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
	class USpringArmComponent* SpringArm{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
	class UCameraComponent* Camera{ nullptr };


	// Weapon Equipment
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerVariables")
	class AWeapon* EquippedWeapon{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
	class AWeapon* ActiveOverlappingWeapon{ nullptr };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for forward/backwards input */
	void MoveForward(float Value);

	/** Called for Right/Left input */
	void MoveRight(float Value);

	// Called to incream MaxWalkSpeed
	void StartSprint();

	// Called to lower MaxWalkspeed back to normal
	void StopSprint();

	// Called to attack
	void LMBDown();

	// Called to attack
	void LMBUp();

	bool bLMBDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerVariables")
	bool bAttacking;





	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	int32 Selection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerVariables")
	class UAnimMontage* CombatMontage;

	// Called to use
	void Use();
	
	// Function to set and get weapon
	void SetEquippedWeapon(AWeapon* WeaponToSet);
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }
	FORCEINLINE void SetActiveOverlappingItem(AWeapon* WeaponOverlapping) { ActiveOverlappingWeapon = WeaponOverlapping; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
	float RightInputValue;

	float JumpVelocity;

	

};
