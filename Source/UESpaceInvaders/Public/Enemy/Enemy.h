// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UEnemyDataAsset;
class AProjectile;

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

	float AttackPercent;

	void OnDead() const;

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:

	FORCEINLINE void SetGridPosition(const int X, const int Y) { GridPosition.X = X; GridPosition.Y = Y; }
	FORCEINLINE FIntVector2 GetGridPosition() const { return GridPosition;}

	void InitializeEnemy(const int X, const int Y, const UEnemyDataAsset* EnemyData);

	void Attack(const TSubclassOf<AProjectile>& ProjectileClass, const float ProjectileSpeed);
};
