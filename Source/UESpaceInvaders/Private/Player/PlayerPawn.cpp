// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile/Projectile.h"

// Sets default values
APlayerPawn::APlayerPawn() :
	MovementSpeed(300.0f),
	bCanControl(true),
	ProjectileSpeed(1500),
	ProjectileSpawnLocationOffset(100)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// Initialization of new Input System
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext,0);
		}		
	}

	PlayerCont = Cast<APlayerController>(GetController());
	
}

void APlayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PlayerCont && SpawnedProjectile && SpawnedProjectile->GetIsActive())
	{
		FVector2d ScreenPos;
		PlayerCont->ProjectWorldLocationToScreen(SpawnedProjectile->GetActorLocation(),ScreenPos);

		if(ScreenPos.Y < 0)
		{
			SpawnedProjectile->DisableProjectile(true);				
		}
	}
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&APlayerPawn::Move);
		EnhancedInputComponent->BindAction(FireAction,ETriggerEvent::Started,this,&APlayerPawn::Fire);
	}

}

void APlayerPawn::Move(const FInputActionValue& Value)
{
	if(!bCanControl) return;
	
	const float InputVal = Value.Get<float>();

	const FVector HorizontalMovement = FVector::RightVector * InputVal * MovementSpeed * GetWorld()->GetDeltaSeconds();

	AddActorWorldOffset(HorizontalMovement);
}

void APlayerPawn::Fire()
{
	if (!bCanControl) return;

	if(SpawnedProjectile)
	{
		if(SpawnedProjectile->GetIsActive()) return;
		
		SpawnedProjectile->SetActorLocation(GetActorLocation() + GetActorForwardVector() * ProjectileSpawnLocationOffset);
		SpawnedProjectile->DisableProjectile(false);
		SpawnedProjectile->SetVelocity(GetActorForwardVector() * ProjectileSpeed);		
	}
	else
	{
		SpawnProjectile();
		SpawnedProjectile->SetVelocity(GetActorForwardVector() * ProjectileSpeed);	
	}	
}


void APlayerPawn::SpawnProjectile()
{
	const auto World = GetWorld();
	if (World && ProjectileToSpawn)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Owner = this;
		
		SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileToSpawn, GetActorLocation() + GetActorForwardVector() * ProjectileSpawnLocationOffset, GetActorRotation(),SpawnParameters);
		if (SpawnedProjectile)
		{
			SpawnedProjectile->SetDestroyOnHit(false);
		}
	}
}
