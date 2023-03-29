// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UESpaceInvadersGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UESPACEINVADERS_API AUESpaceInvadersGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	int PlayersScore;

public:

	FORCEINLINE void IncreaseScore(int Score) {PlayersScore += Score; UE_LOG(LogTemp,Log,TEXT("Score : %i"),PlayersScore)}

private:

	FTimerHandle UFOSpawnTimerHandle;
	
};
