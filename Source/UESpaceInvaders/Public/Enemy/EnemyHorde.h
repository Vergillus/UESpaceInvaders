// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyHorde.generated.h"

class AEnemy;
class UEnemyDataAsset;
class APlayerController;

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

	UPROPERTY(EditDefaultsOnly, Category= " Enemy Grid")
	TSubclassOf<AEnemy> EnemyToSpawn;

	UPROPERTY(EditDefaultsOnly, Category= " Enemy Grid")
	TArray<UEnemyDataAsset*> EnemyData;

	UPROPERTY(EditDefaultsOnly, Category= " Enemy Grid")
	int EnemyGridWidth;

	UPROPERTY(EditDefaultsOnly, Category= " Enemy Grid")
	int EnemyGridHeight;

	UPROPERTY(EditDefaultsOnly, Category= " Enemy Grid")
	int GridCellSize;

	int GridCellHalfSize;

	UPROPERTY(VisibleAnywhere, Category= "Debug")
	TObjectPtr<AActor> TopLeftEnemy;

	UPROPERTY(VisibleAnywhere, Category= "Debug")
	TObjectPtr<AActor> TopRightEnemy;

	UFUNCTION(BlueprintCallable)
	void SpawnEnemies();

	void SpawnEnemiesWithDelay();

	void Deneme();

	int DenX,DenY;

	FTimerHandle SpawnEnemiesTimerHandle;
	FTimerHandle SpawnEnemiesTimerPauseHandle;

	TArray<AEnemy*> SpawnedEnemies;

	TArray<uint8> Grid;

	int32 GetIndexFrom2D(const int X, const int Y) const;

	UEnemyDataAsset* GetEnemyData(const int Row) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Movement)
	float MovementInterval;
	float MovementCounter;

	float VerticalMovementDelay;
	float VerticalMovementCounter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Movement)
	int MovementInput;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Movement)
	float WorldXPosToGameOver;

	void Move(float DeltaTime);
	void CheckGameOver();

	UPROPERTY(VisibleAnywhere, Category= "Debug")
	int ViewportSizeX;

	UPROPERTY(VisibleAnywhere, Category= "Debug")
	int ViewportSizeY;

	bool bCanMove;

	TObjectPtr<APlayerController> PlayerController;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void EnemyDead(const int GridPosX, const int GridPosY);

};
