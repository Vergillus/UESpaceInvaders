// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UESpaceInvadersGameModeBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerRespawned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemySpawnCompleted);

class AUFO;
class APlayerController;

/**
 * 
 */
UCLASS()
class UESPACEINVADERS_API AUESpaceInvadersGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	int PlayersScore;

	int PlayerLives = 3;

public:

	/** GM Events */
	//---------------------------------------------
	FOnPlayerDead OnPlayerDead;
	FOnPlayerRespawned OnPlayerRespawned;
	FOnEnemySpawnCompleted OnEnemySpawnCompleted;
	//---------------------------------------------

	FORCEINLINE void IncreaseScore(const int Score) {PlayersScore += Score; }

	void PlayerDead();

	void EnemySpawnCompleted();

	void StartUfoSpawnTimer();
	
protected:	
	
	UPROPERTY(EditAnywhere)
	float PlayerRespawnDelay;

	TObjectPtr<APlayerController> PlayerCont;

private:

	int ViewportX,ViewportY;

	/** Timer Handles */
	//---------------------------------------------
	FTimerHandle UFOSpawnTimerHandle;
	FTimerHandle PlayerRespawnTimerHandle;
	//---------------------------------------------

	UPROPERTY(EditDefaultsOnly, Category= "UFO")
	TSubclassOf<AUFO> UFOToSpawn;	
	UPROPERTY(EditDefaultsOnly, Category= "UFO")
	float UFOSpawnMinDelay;
	UPROPERTY(EditDefaultsOnly, Category= "UFO")
	float UFOSpawnMaxDelay;

	UPROPERTY(EditDefaultsOnly, Category= "UFO")
	float UFOSpawnPosScreenYOffset;

	void SpawnUFO();

	/** Timer Handle Functions */
	//---------------------------------------------
	void RespawnPlayer() const;
	//---------------------------------------------

	FORCEINLINE bool IsPlayerHaveEnoughLives() const {return PlayerLives > 0;}
	
};
