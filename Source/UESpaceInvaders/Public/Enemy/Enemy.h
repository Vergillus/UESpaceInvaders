// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UEnemyDataAsset;

UCLASS()
class UESPACEINVADERS_API AEnemy : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category= Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(EditAnywhere, Category= Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	FIntVector2 GridPosition;
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:	

	int32 Score;

	void Attack();

	void OnDead();

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:

	FORCEINLINE void SetGridPosition(const int X, const int Y) { GridPosition.X = X; GridPosition.Y = Y; }

	void InitializeEnemy(const int X, const int Y, const UEnemyDataAsset* EnemyData);
};
