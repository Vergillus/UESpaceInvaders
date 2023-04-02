// Fill out your copyright notice in the Description page of Project Settings.


#include "Bunker/BunkerPiece.h"

// Sets default values
ABunkerPiece::ABunkerPiece() :
	Health(4)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	RootComponent = Mesh;
}

void ABunkerPiece::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = Health;

	DynamicMaterial = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0),nullptr);
	if (DynamicMaterial)
	{
		Mesh->SetMaterial(0,DynamicMaterial);
		DynamicMaterial->SetScalarParameterValue(TEXT("Erosion"), 1.0f);
	}
}



float ABunkerPiece::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                               AActor* DamageCauser)
{
	CurrentHealth = FMath::Max(0, CurrentHealth - DamageAmount);

	// Change material properties
	const float HealthPercent = CurrentHealth / (Health * 1.0f);
	DynamicMaterial->SetScalarParameterValue(TEXT("Erosion"), HealthPercent);
	
	if (CurrentHealth == 0)
	{	
		OnDead();
	}
	
	return DamageAmount;
}

void ABunkerPiece::OnDead()
{
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DynamicMaterial = nullptr;
}
