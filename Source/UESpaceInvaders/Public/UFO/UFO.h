// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UFO.generated.h"

class USphereComponent;
class APlayerController;
class USoundBase;

UCLASS()
class UESPACEINVADERS_API AUFO : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category= "Components")
	TObjectPtr<USphereComponent> CollisionSphere;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category= "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, meta=( AllowPrivateAccess = "true"), Category= "Components")
	TObjectPtr<UAudioComponent> UFOAudioComp;

	UPROPERTY(EditDefaultsOnly, meta=( AllowPrivateAccess = "true"), Category= "VFX")
	TObjectPtr<USoundBase> DeathSound;

	UPROPERTY(EditDefaultsOnly, meta=( AllowPrivateAccess = "true"), Category= "VFX")
	TObjectPtr<UParticleSystem> DeathParticle;	

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int Score;
	
public:	
	// Sets default values for this actor's properties
	AUFO();

	void InitializeUfo(TObjectPtr<APlayerController> PlayerCont, FVector MovementDir);

	void Move(const float DeltaTime);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category= "Movement")
	float MovementSpeed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	int32 ViewportSizeX, ViewportSizeY;
	
	FVector MovementDirection;

	TObjectPtr<APlayerController> PlayerController;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void OnDead(bool bReceivedDamage = false);

};
