// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonCharacter.generated.h"

UCLASS()
class COMBATPROJECT_API AThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AThirdPersonCharacter();





	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
	class USpringArmComponent* SpringArm{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
	class UCameraComponent* Camera{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerVariables")
	class AWeapon* EquippedWeapon{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
	class AWeapon* ActiveOverlappingWeapon{ nullptr };

	// Function to set and get weapon
	void SetEquippedWeapon(AWeapon* WeaponToSet);
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }
	FORCEINLINE void SetActiveOverlappingItem(AWeapon* WeaponOverlapping) { ActiveOverlappingWeapon = WeaponOverlapping; }


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
	class UCapsuleComponent* HealthCollider{ nullptr };


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

	void Use();

	void ForwardLock();

	void LeftMouseButtonPress();

	void LeftMouseButtonRelease();

	void RightMouseButtonPress();

	void RightMouseButtonRelease();

	void SprintPressed();

	void SprintReleased();

	void Dodge();

	void ChangeDebugMode();


	UFUNCTION()
	virtual void HealthColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	
	float InterpSpeed;
	bool bInterpToEnemy;
	void SetInterpToEnemy(bool Interp);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerVariables | Combat")
	class AClassicEnemy* CombatTarget;

	FORCEINLINE void SetCombatTarget(AClassicEnemy* Target) { CombatTarget = Target; }



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Logic")
	bool bDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stats")
	float Health = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stats")
	float MaxHealth = 150.f;


	// Animation Blueprint Components

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	int runTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	int pre_runTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	int DebugMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisRun;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisWalk;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisHoldSpace;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisRoll;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisDodge;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bIsForwardLock;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bIsAttackSecondary;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisDebug;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisLocomotion;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisSnapRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisThrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	float inputX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	float inputY;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	float inputX_Local;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	float inputY_Local;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	float Speed_Run;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	float Speed_Jog; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	float Speed_Walk;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	FVector inputVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	FVector inputVector_World;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	FVector LineEndTemp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	FVector DamagePosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool isInvisible = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool isDamaging = false;

	// Draw DebugVectors for us
	UFUNCTION()
	void DebugVector();

	FVector GetEndPoint(float input);

	// Snap Rotation Fn
	void SnapRotationFn();

	void RunorWalk();

	void SetForwardLock(bool input);

	void InterpToEnemy(float Delta);

	bool Retrigger;
	float RetriggerDelay = 0.2;
	float RetriggerClock = 0.f;




	UFUNCTION(BlueprintCallable)
	void ResetDodge();

	UFUNCTION(BlueprintCallable)
	void ResetRoll();

	UFUNCTION(BlueprintCallable)
	void SnapRotation();

	UFUNCTION(BlueprintCallable)
	void ResetAttack();

	UFUNCTION()
	void LoseHealth(float Value);


};
