// Copyright Epic Games, Inc. All Rights Reserved.


#include "UESpaceInvadersGameModeBase.h"

#include "SIGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile/Projectile.h"
#include "UFO/UFO.h"
#include "UI/InGameUI.h"


void AUESpaceInvadersGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<USIGameInstance>(UGameplayStatics::GetGameInstance(this));

	
	if(InGameUIToSpawn && GameInstance)
	{
		InGameUIInstance = CreateWidget<UInGameUI>(GetWorld(), InGameUIToSpawn);
		if (InGameUIInstance)
		{
			InGameUIInstance->AddToViewport();
			InGameUIInstance->ChangeScore(GameInstance->GetScore());

			if(GameInstance->GetLives() != GameInstance->GetMaxLives())
			{
				InGameUIInstance->OnLevelChanged(GameInstance->GetLives());						
			}
		}
	}

	
}

void AUESpaceInvadersGameModeBase::IncreaseScore(const int Score)
{
	if(Score == 0 || !GameInstance) return;	
	
	GameInstance->IncreaseScore(Score);

	InGameUIInstance->ChangeScore(GameInstance->GetScore());
}

void AUESpaceInvadersGameModeBase::PlayerDead()
{
	if(!GameInstance) return;

	GameInstance->DecreaseLives();
	InGameUIInstance->OnPlayerDead(GameInstance->GetLives());

	if (!IsPlayerHaveEnoughLives())
	{
		UE_LOG(LogTemp, Warning, TEXT("Player has no more lives GAME OVER"));

		// TODO: Load GAme OVer Scene
	}

	if(OnPlayerDead.IsBound())
	{
		OnPlayerDead.Broadcast();

		GetWorldTimerManager().SetTimer(PlayerRespawnTimerHandle, this, &AUESpaceInvadersGameModeBase::RespawnPlayer, PlayerRespawnDelay);
	}	
}

void AUESpaceInvadersGameModeBase::EnemySpawnCompleted()
{
	if (OnEnemySpawnCompleted.IsBound())
	{
		OnEnemySpawnCompleted.Broadcast();
	}

	PlayerCont = UGameplayStatics::GetPlayerController(this,0);
	if (PlayerCont)
	{
		PlayerCont->GetViewportSize(ViewportX,ViewportY);
	}	

	StartUfoSpawnTimer();
}

void AUESpaceInvadersGameModeBase::RespawnPlayer() const
{
	if (OnPlayerRespawned.IsBound())
	{

		TArray<AActor*> ProjectileActors;
		UGameplayStatics::GetAllActorsOfClass(this,AProjectile::StaticClass(),ProjectileActors);
		for (AActor* Projectile : ProjectileActors)
		{
			Projectile->Destroy();
		}
		
		OnPlayerRespawned.Broadcast();
	}
}

bool AUESpaceInvadersGameModeBase::IsPlayerHaveEnoughLives() const
{
	return GameInstance->GetLives() > 0;
}

void AUESpaceInvadersGameModeBase::StartUfoSpawnTimer()
{
	if(!UFOToSpawn) return;
	
	const float InRate = FMath::RandRange(UFOSpawnMinDelay, UFOSpawnMaxDelay);
	GetWorldTimerManager().SetTimer(UFOSpawnTimerHandle, this, &AUESpaceInvadersGameModeBase::SpawnUFO,InRate);	
}


void AUESpaceInvadersGameModeBase::SpawnUFO()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation, WorldDir;

	const float ScreenX = FMath::RandRange(0.0f,1.0f) > 0.5f ? ViewportX : 0;
	
	if(PlayerCont->DeprojectScreenPositionToWorld(ScreenX,UFOSpawnPosScreenYOffset,SpawnLocation,WorldDir))
	{
		SpawnLocation.Z = 0.0f;
		if(const auto UFOClone = GetWorld()->SpawnActor<AUFO>(UFOToSpawn,SpawnLocation,FRotator::ZeroRotator,SpawnParameters))
		{
			const FVector UFOMovementDir = ScreenX <= 0 ? FVector::RightVector : FVector::LeftVector;
			UFOClone->InitializeUfo(PlayerCont,UFOMovementDir);
		}
	}	
}

