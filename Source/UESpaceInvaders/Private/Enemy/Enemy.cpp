// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "Components/SphereComponent.h"
#include "Enemy/EnemyDataAsset.h"
#include "Enemy/EnemyHorde.h"
#include "Kismet/GameplayStatics.h"
#include "UESpaceInvaders/UESpaceInvadersGameModeBase.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	RootComponent = CollisionSphere;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetGenerateOverlapEvents(false);

}

void AEnemy::InitializeEnemy(const int X, const int Y, const UEnemyDataAsset* EnemyData)
{
	SetGridPosition(X,Y);

	Mesh->SetStaticMesh(EnemyData->Mesh);
	Score = EnemyData->ReceivedScore;
	AttackPercent = EnemyData->AttackChancePercent;
}

void AEnemy::Attack() const
{
	if (AttackPercent <= FMath::RandRange(0.0f,1.0f))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Pos %i __ %i is Attacking"),GridPosition.X, GridPosition.Y);		
	}
	
}

void AEnemy::OnDead()
{
	if(const auto EnemyHorde = Cast<AEnemyHorde>(GetOwner()))
	{
		// Notify EnemyHorde that this Enemy is dead
		EnemyHorde->EnemyDead(GridPosition.X, GridPosition.Y);

		// TODO:Play Sound

		// TODO:Play Particle
		
		if(const auto GameMode = Cast<AUESpaceInvadersGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			GameMode->IncreaseScore(Score);
		}
	}
}

void AEnemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//UE_LOG(LogTemp,Warning,TEXT("%s overlapas %s"), *GetName(), *OtherActor->GetName());
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	OnDead();
	
	return DamageAmount;
}




