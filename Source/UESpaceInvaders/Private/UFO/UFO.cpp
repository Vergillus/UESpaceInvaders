// Fill out your copyright notice in the Description page of Project Settings.


#include "UFO/UFO.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UESpaceInvaders/UESpaceInvadersGameModeBase.h"

// Sets default values
AUFO::AUFO() :
	MovementSpeed(300)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	RootComponent = CollisionSphere;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UFOAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("UFO Sound"));
}


// Called when the game starts or when spawned
void AUFO::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUFO::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);

	if (PlayerController)
	{
		// Check if the UFO has reached the edges of the screen...
		const FVector ScreenEdgeCheckPos = GetActorLocation() - MovementDirection * CollisionSphere->
			GetScaledSphereRadius();
		FVector2D ScreenLoc;
		PlayerController->ProjectWorldLocationToScreen(ScreenEdgeCheckPos, ScreenLoc);

		// If so declare UFO as dead without giving points to the player
		if (MovementDirection == FVector::RightVector && ScreenLoc.X > ViewportSizeX) // Check for right edge of the screen
		{
			OnDead();
			return;
		}
		if (MovementDirection == FVector::LeftVector && ScreenLoc.X < 0) // Check for left edge of the screen
		{
			OnDead();
			return;
		}
	}
}

void AUFO::InitializeUfo(const TObjectPtr<APlayerController> PlayerCont, const FVector MovementDir)
{
	PlayerController = PlayerCont;
	MovementDirection = MovementDir;

	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
}


void AUFO::Move(const float DeltaTime)
{
	const FVector DeltaLoc = MovementDirection * MovementSpeed * DeltaTime;
	AddActorWorldOffset(DeltaLoc);
}

float AUFO::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                       AActor* DamageCauser)
{
	OnDead(true);
	return DamageAmount;
}

void AUFO::OnDead(const bool bReceivedDamage)
{
	// Notify GameMode that UFO is dead.
	if (const auto GM = Cast<AUESpaceInvadersGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		if (bReceivedDamage)
		{
			GM->IncreaseScore(Score);

			if(DeathSound)
			{
				UGameplayStatics::PlaySound2D(this, DeathSound);		
			}
			if (DeathParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle,GetActorLocation());
			}
		}

		GM->StartUfoSpawnTimer();
	}	

	Destroy();
}
