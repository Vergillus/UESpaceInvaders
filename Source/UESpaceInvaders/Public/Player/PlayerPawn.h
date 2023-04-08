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
class AUESpaceInvadersGameModeBase;

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

private:
	/** Input Actions */
	//------------------------------------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category =  "Input")
	UInputAction* MoveAction;	

	UPROPERTY(EditDefaultsOnly, Category =  "Input")
	UInputAction* FireAction;
	//------------------------------------------------------------------------------------------------

	/** Input Action Functions */
	//---------------------------------------
	void Move(const FInputActionValue& Value);
	void Fire();
	//---------------------------------------

	/** Movement Related */
	//---------------------------------------
	UPROPERTY(EditAnywhere, Category= "Movement")
	float MovementSpeed;

	bool bCanControl;

	UFUNCTION()
	FORCEINLINE void ToggleCanControl() {bCanControl = !bCanControl;}
	//---------------------------------------

	/** Projectile */
	//------------------------------------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category= "Projectile")
	TSubclassOf<AProjectile> ProjectileToSpawn;

	UPROPERTY(VisibleAnywhere, Category= "Projectile")
	TObjectPtr<AProjectile> SpawnedProjectile;

	void SpawnProjectile();

	UPROPERTY(EditAnywhere, Category= "Projectile")
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, Category= "Projectile")
	float ProjectileSpawnLocationOffset;	
	//------------------------------------------------------------------------------------------------

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerCont;

	/** Bounding box of the player.
	 * Used to check whether player has reached the edges of the screen 
	 */
	FVector BoxExtend;

	UPROPERTY()
	TObjectPtr<AUESpaceInvadersGameModeBase> GameModeBase;

	/** Receive Damage Related */
	//------------------------------------------------------------------------------------------------
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void OnDead();
	UFUNCTION()

	/** Called via GameMode when respawn timer finished. */
	void OnRespawn();
	void DisablePlayer(bool bIsDisabled);

	UPROPERTY(EditAnywhere, Category= "VFX")
	TObjectPtr<USoundBase> DeathSound;

	UPROPERTY(EditAnywhere, Category= "VFX")
	TObjectPtr<UParticleSystem> DeathParticle;

	FVector InitialPosition;
	//------------------------------------------------------------------------------------------------
	
	UPROPERTY(EditAnywhere, Category= "VFX")
	TObjectPtr<USoundBase> FireSound;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
