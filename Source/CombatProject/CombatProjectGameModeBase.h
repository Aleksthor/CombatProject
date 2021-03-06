// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CombatProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class COMBATPROJECT_API ACombatProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ACombatProjectGameModeBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;




};
