// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BunkerPiece.generated.h"

UCLASS()
class UESPACEINVADERS_API ABunkerPiece : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

protected:
	virtual void BeginPlay() override;
	
public:	
	// Sets default values for this actor's properties
	ABunkerPiece();	

	UPROPERTY(EditAnywhere)
	int Health;

	UPROPERTY(VisibleAnywhere)
	int CurrentHealth;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:

	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;

	void OnDead();
};
