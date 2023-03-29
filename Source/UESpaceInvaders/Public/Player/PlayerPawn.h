// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "PlayerPawn.generated.h"

class UInputMappingContext;
class UInputAction;
class AProjectile;
class APlayerController;

UCLASS()
class UESPACEINVADERS_API APlayerPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true") , Category= "Input")
	TObjectPtr<UStaticMeshComponent> Mesh;

	/** New input system */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true") , Category= "Input")
	UInputMappingContext* PlayerMappingContext;

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	/** Input Actions */
	//------------------------------------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category =  "Input")
	UInputAction* MoveAction;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category =  "Input")
	UInputAction* FireAction;
	//------------------------------------------------------------------------------------------------

	/** Input Action Functions */
	//---------------------------------------
	void Move(const FInputActionValue& Value);
	void Fire();
	//---------------------------------------

	UPROPERTY(EditAnywhere, Category= "Movement")
	float MovementSpeed;

	bool bCanControl;

	/** Projectile */
	//------------------------------------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category= "Projectile")
	TSubclassOf<AProjectile> ProjectileToSpawn;

	TObjectPtr<AProjectile> SpawnedProjectile;

	void SpawnProjectile();

	UPROPERTY(EditAnywhere, Category= "Projectile")
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, Category= "Projectile")
	float ProjectileSpawnLocationOffset;	
	//------------------------------------------------------------------------------------------------

	TObjectPtr<APlayerController> PlayerCont;	

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
