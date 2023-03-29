// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Projectile.h"

#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile() :
	bIsActive(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	RootComponent = CollisionSphere;
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));

}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetOwner())
	{
		UE_LOG(LogTemp,Warning,TEXT("Collide with %s"), *OtherActor->GetName());
		OtherActor->TakeDamage(1, FDamageEvent(UDamageType::StaticClass()),nullptr,nullptr);

		if(bShouldDestroyOnHit)
		{
			Destroy();
		}
		else
		{
			DisableProjectile(true);
		}
	}
}

void AProjectile::SetDestroyOnHit(bool NewVal)
{
	bShouldDestroyOnHit = NewVal;

	if(NewVal == true)
	{
		SetLifeSpan(5.0f);		
	}
}

void AProjectile::SetVelocity(const FVector& NewVelocity)
{
	MovementComponent->Velocity = NewVelocity;	
}

void AProjectile::DisableProjectile(bool bIsDisabled)
{
	if (!bIsDisabled)
	{
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		MovementComponent->StopMovementImmediately();
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);		
	}

	bIsActive = !bIsDisabled;
	Mesh->SetVisibility(!bIsDisabled);
}
