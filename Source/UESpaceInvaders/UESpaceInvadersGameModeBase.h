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
class UInGameUI;
class USIGameInstance;
class USISaveGame;

/**
 * 
 */
UCLASS()
class UESPACEINVADERS_API AUESpaceInvadersGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	/** GM Events */
	//---------------------------------------------
	FOnPlayerDead OnPlayerDead;
	FOnPlayerRespawned OnPlayerRespawned;
	FOnEnemySpawnCompleted OnEnemySpawnCompleted;
	//---------------------------------------------

	UFUNCTION()
	FORCEINLINE void IncreaseScore(const int Score) const;

	/** Win/Lose */
	//---------------------------------------------
	void PlayerDead();
	void PlayerWin();
	//---------------------------------------------

	void EnemySpawnCompleted();

	void StartUfoSpawnTimer();
	
protected:	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	float PlayerRespawnDelay;

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerCont;

private:

	int ViewportX,ViewportY;

	/** Timer Handles */
	//---------------------------------------------
	FTimerHandle UFOSpawnTimerHandle;
	FTimerHandle PlayerRespawnTimerHandle;
	//---------------------------------------------

	/** UFO Related */
	//---------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category= "UFO")
	TSubclassOf<AUFO> UFOToSpawn;	
	UPROPERTY(EditDefaultsOnly, Category= "UFO")
	float UFOSpawnMinDelay;
	UPROPERTY(EditDefaultsOnly, Category= "UFO")
	float UFOSpawnMaxDelay;

	UPROPERTY(EditDefaultsOnly, Category= "UFO")
	float UFOSpawnPosScreenYOffset;

	void SpawnUFO() const;
	//---------------------------------------------

	/** Timer Handle Functions */
	//---------------------------------------------
	void RespawnPlayer() const;
	//---------------------------------------------

	bool IsPlayerHaveEnoughLives() const;

	/** In Game UI */
	//---------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category= "UI", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<UInGameUI> InGameUIToSpawn;

	TObjectPtr<UInGameUI> InGameUIInstance;
	//---------------------------------------------

	UPROPERTY()
	TObjectPtr<USIGameInstance> GameInstance;
	
};
