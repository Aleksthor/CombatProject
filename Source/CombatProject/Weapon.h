// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Pickup		UMETA(DisplayName = "Pickup"),
	EWS_Equipped	UMETA(DisplayName = "Equipped"),

	EWS_Max	UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class COMBATPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	EWeaponState WeaponState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeaponVariables")
	class USphereComponent* PickupCollider{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeaponVariables")
	class UBoxComponent* AttackCollider{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeaponVariables")
	class USkeletalMeshComponent* WeaponMesh{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponVariables | Sound")
	class USoundCue* OnEquipSound{ nullptr };

	void Equip(class ACharacter* Char);



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponVariables")
	float WeaponDamage = 17.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponVariables")
	bool isOverlapping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponVariables")
	int TimesGivenDamage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponVariables")
	class AClassicEnemy* ClassicEnemy;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapPickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEndPickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	UFUNCTION()
	void OnOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEndAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponVariables")
	bool bEquipSetVisibility;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeaponVariables")
	bool Equipped = false;


	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State; }
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }

};
