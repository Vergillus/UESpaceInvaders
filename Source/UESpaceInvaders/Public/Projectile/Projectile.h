// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"


class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class UESPACEINVADERS_API AProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category= "Components")
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category= "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category= "Components")
	TObjectPtr<UProjectileMovementComponent> MovementComponent;	
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:	
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(VisibleAnywhere)
	bool bShouldDestroyOnHit;

	UPROPERTY(VisibleAnywhere)
	bool bIsActive;

	UPROPERTY(EditDefaultsOnly);
	float LifeTime;

public:

	void SetDestroyOnHit(bool NewVal);

	void SetVelocity(const FVector& NewVelocity) const;
	void DisableProjectile(bool bIsDisabled);

	FORCEINLINE bool GetIsActive() const { return bIsActive;}
};
