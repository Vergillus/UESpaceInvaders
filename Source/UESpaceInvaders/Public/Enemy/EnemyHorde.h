// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyHorde.generated.h"

class AEnemy;
class UEnemyDataAsset;
class APlayerController;
class AProjectile;
class UCurveFloat;
class USoundBase;
class UParticleSystem;

UCLASS()
class UESPACEINVADERS_API AEnemyHorde : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyHorde();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category= "Debug")
	TObjectPtr<AEnemy> TopLeftEnemy;

	UPROPERTY(VisibleAnywhere, Category= "Debug")
	TObjectPtr<AEnemy> TopRightEnemy;

	/** Spawn Related */
	//---------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category= "Enemy Grid")
	TSubclassOf<AEnemy> EnemyToSpawn;

	/** A list that contains enemy data*/
	UPROPERTY(EditDefaultsOnly, Category= "Enemy Grid")
	TArray<UEnemyDataAsset*> EnemyData;

	UPROPERTY(EditDefaultsOnly, Category= "Enemy Grid")
	int EnemyGridWidth;

	UPROPERTY(EditDefaultsOnly, Category= "Enemy Grid")
	int EnemyGridHeight;

	UPROPERTY(EditDefaultsOnly, Category= "Enemy Grid")
	int GridCellSize;

	int GridCellHalfSize;

	/** Spawns enemies in a grid with delay instead of spawning in a single frame */
	void SpawnEnemiesWithDelay();
	void EnemySpawnComplete();

	/** Grid X and Y indexes. Used in spawn process */
	int GridX,GridY;

	/** Delay amount for enemy spawn process */
	UPROPERTY(EditAnywhere, Category= "Enemy Grid")
	float EnemySpawnDelay;
	
	UEnemyDataAsset* EnemyDataAsset;

	FTimerHandle SpawnEnemiesTimerHandle;

	/** Returns 1D array index from 2D array index */
	int32 GetIndexFrom2D(const int X, const int Y) const;

	/** Returns EnemyData according to grid row index */
	UEnemyDataAsset* GetEnemyData(const int Row) const;

	/** List to store spawned enemies */
	TArray<AEnemy*> SpawnedEnemies;
	//---------------------------------------------------------------	


	/** Movement Related */
	//---------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Movement)
	TObjectPtr<UCurveFloat> MovementIntervalCurve;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Movement)
	float CurrentMovementInterval;
	float MovementCounter;

	/** Delay the movement after Vertical movement occurs. */
	float VerticalMovementDelay;
	float VerticalMovementCounter;
	
	UPROPERTY(VisibleAnywhere, Category= Movement)
	int8 MovementInput;

	/** Game will be over once any child Actor(Enemy) reaches the value below */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Movement)
	float WorldXPosToGameOver;

	void Move(float DeltaTime);
	void CheckGameOver();

	/** Changes the movement interval according the alive enemy count in the horde */
	void ChangeMovementInterval();

	UPROPERTY(VisibleAnywhere, Category= "Debug")
	int ViewportSizeX;

	UPROPERTY(VisibleAnywhere, Category= "Debug")
	int ViewportSizeY;

	bool bCanMove;

	UFUNCTION()
	FORCEINLINE void ToggleCanMove() { bCanMove = !bCanMove;};

	TObjectPtr<APlayerController> PlayerController;

	//---------------------------------------------------------------

	/** Attack Related */
	//---------------------------------------------------------------
	FTimerHandle AttackTimerHandle;

	/** Max and min attack rate. Used to get random value between them */
	UPROPERTY(EditAnywhere, Category= "Attack")
	float MinAttackRate;

	UPROPERTY(EditAnywhere, Category= "Attack")
	float MaxAttackRate;

	UPROPERTY(EditAnywhere, Category= "Attack")
	TSubclassOf<AProjectile> ProjectileToSpawn;

	UPROPERTY(EditAnywhere, Category= "Attack")
	float MinProjectileSpeed;

	UPROPERTY(EditAnywhere, Category= "Attack")
	float MaxProjectileSpeed;

	UPROPERTY(EditAnywhere, Category= "Attack | VFX")
	TObjectPtr<USoundBase> DeathSound;

	UPROPERTY(EditAnywhere, Category= "Attack | VFX")
	TObjectPtr<UParticleSystem> DeathParticle;
	
	/** List to check whether enemy has other enemies in front in order to attack */
	UPROPERTY(VisibleAnywhere, Category= "Debug")
	TArray<uint8> AttackLookupList;
	
	void OrderAttack();
	void StartAttackTimer();	
	//---------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, Category= "Debug")
	int32 AliveEnemyCnt;

	/** Finds a new top left/right enemy*/
	TObjectPtr<AEnemy> GetNewTopEnemy(int32 GridPosX, int32 GridPosY, bool bIsTopLeft);

	/** If there is no alive enemy then the player wins */
	FORCEINLINE bool CheckWin() const { return AliveEnemyCnt == 0; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void EnemyDead(const int GridPosX, const int GridPosY);

	FORCEINLINE TObjectPtr<USoundBase> GetDeathSound() const {return  DeathSound;}
	FORCEINLINE TObjectPtr<UParticleSystem> GetDeathParticle() const {return  DeathParticle;}

};
