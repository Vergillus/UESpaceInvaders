// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "Components/SphereComponent.h"
#include "Enemy/EnemyDataAsset.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	RootComponent = CollisionSphere;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

}

void AEnemy::InitializeEnemy(const int X, const int Y, const UEnemyDataAsset* EnemyData)
{
	SetGridPosition(X,Y);

	Mesh->SetStaticMesh(EnemyData->Mesh);
	Score = EnemyData->ReceivedScore;
}

void AEnemy::Attack()
{
}

void AEnemy::OnDead()
{
}

void AEnemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	UE_LOG(LogTemp,Warning,TEXT("%s overlapas %s"), *GetName(), *OtherActor->GetName());
}




