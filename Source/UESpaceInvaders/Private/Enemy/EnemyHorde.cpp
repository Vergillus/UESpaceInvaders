// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyHorde.h"

#include "SIGameInstance.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemyDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UESpaceInvaders/UESpaceInvadersGameModeBase.h"


#pragma region Engine Functions
// Sets default values
AEnemyHorde::AEnemyHorde() :
	EnemyGridWidth(12),
	EnemyGridHeight(5),
	GridCellSize(100),
	EnemySpawnDelay(0.05f),
	MovementCounter(0.0f),
	VerticalMovementDelay(0.5f),
	MovementInput(1),
	WorldXPosToGameOver(-200),
	bCanMove(false),
	MinAttackRate(1.0f),
	MaxAttackRate(2.0f),
	MinProjectileSpeed(300),
	MaxProjectileSpeed(1000),
	AliveEnemyCnt(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyHorde::BeginPlay()
{
	Super::BeginPlay();

	// Change start position based on the number of levels passed.
	if(const auto GI = Cast<USIGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		const int XOffsetMulti = FMath::Min(GI->GetCurrentLevel(), GI->GetMaxLevel());
		const FVector NewLocation = GetActorLocation() - FVector::ForwardVector * GridCellSize * XOffsetMulti;
		SetActorLocation(NewLocation);
	}

	// Randomly assign MovementInput
	MovementInput = FMath::RandRange(0, 100) > 50 ? 1 : -1;
	
	GridCellHalfSize = GridCellSize * 0.5f;

	// Initiate spawn process
	//--------------------------------------------
	if (EnemyData.IsEmpty()) return;

	AttackLookupList.SetNum(EnemyGridWidth * EnemyGridHeight);

	EnemyDataAsset = GetEnemyData(0);

	// Start enemy spawning process
	GetWorldTimerManager().SetTimer(SpawnEnemiesTimerHandle, this, &AEnemyHorde::SpawnEnemiesWithDelay, EnemySpawnDelay,
	                                true);
	//--------------------------------------------

	// Bind GM Delegates
	if (const auto GM = Cast<AUESpaceInvadersGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GM->OnPlayerDead.AddDynamic(this, &AEnemyHorde::ToggleCanMove);
		GM->OnPlayerRespawned.AddDynamic(this, &AEnemyHorde::ToggleCanMove);
	}
}

// Called every frame
void AEnemyHorde::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanMove)
	{
		Move(DeltaTime);
	}
}
#pragma endregion 

// Moves the enemy left/right/down
void AEnemyHorde::Move(float DeltaTime)
{
	// Delay/Stop movement after vertical movement happens for a given interval
	if (VerticalMovementCounter > 0)
	{
		VerticalMovementCounter -= DeltaTime;
		return;
	}

	MovementCounter += DeltaTime;

	// Can move??
	if (MovementCounter >= CurrentMovementInterval)
	{
		FVector LocationOffset(0, GridCellSize, 0);
		LocationOffset *= MovementInput;

		// Boundary location of the enemy grid. Used to check whether enemy horde has reached the screen's edge
		FVector BoundaryLocation = MovementInput > 0
			                           ? TopRightEnemy->GetActorLocation()
			                           : TopLeftEnemy->GetActorLocation();
		BoundaryLocation += LocationOffset;
		BoundaryLocation.Y += GridCellHalfSize * MovementInput;		

		// Get the screen position of BoundaryLocation
		FVector2D ScreenPosition;
		PlayerController->ProjectWorldLocationToScreen(BoundaryLocation, ScreenPosition, true);

		// Is the enemy horde reached the screen edge
		if (ScreenPosition.X > ViewportSizeX || ScreenPosition.X < -GridCellHalfSize * 0.5f)
		{
			// Change the movement direction
			MovementInput *= -1;

			// Move Vertically
			AddActorWorldOffset(FVector{-GridCellSize * 0.5f, 0, 0});

			VerticalMovementCounter = VerticalMovementDelay;

			CheckGameOver();

			return;
		}

		// Enemy horde has not reached to screen's edge so continue moving to same direction
		AddActorWorldOffset(LocationOffset);
		MovementCounter = 0.0f;
	}
}

void AEnemyHorde::EnemyDead(const int GridPosX, const int GridPosY)
{
	AliveEnemyCnt--;
	const int EnemyIndex = GetIndexFrom2D(GridPosX, GridPosY);

	// Is Player Won?
	if (CheckWin())
	{
		//Notify GM that the player is WON
		if (const auto GM = Cast<AUESpaceInvadersGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			bCanMove = false;
			SpawnedEnemies[EnemyIndex]->Destroy();
			SpawnedEnemies[EnemyIndex] = nullptr;
			GM->PlayerWin();
			return;			
		}				
	}

	AttackLookupList[EnemyIndex] = 0;

	if (GridPosX == EnemyGridHeight - 1) // Either current top left or right enemy is dead find a new one
	{
		if (SpawnedEnemies[EnemyIndex] == TopLeftEnemy)
		{
			TopLeftEnemy = GetNewTopEnemy(GridPosX, GridPosY, true);
		}
		else if (SpawnedEnemies[EnemyIndex] == TopRightEnemy)
		{
			TopRightEnemy = GetNewTopEnemy(GridPosX, GridPosY, false);
		}
	}

	SpawnedEnemies[EnemyIndex]->Destroy();
	SpawnedEnemies[EnemyIndex] = nullptr;

	ChangeMovementInterval();

	//SpawnedEnemies.RemoveAt(EnemyIndex);
}


#pragma region Spawn Related
void AEnemyHorde::SpawnEnemiesWithDelay()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Owner = this;

	const FAttachmentTransformRules AttachmentTransformRules{EAttachmentRule::KeepWorld, true};

	// Get a new EnemyDataAsset
	if (GridX > EnemyDataAsset->MaxSpawnRowIndex)
	{
		EnemyDataAsset = GetEnemyData(GridX);
	}

	const FVector SpawnPos = GetActorLocation() + FVector(GridX * GridCellSize, GridY * GridCellSize, 0);

	if (AEnemy* EnemyClone = GetWorld()->SpawnActor<AEnemy>(EnemyToSpawn, SpawnPos, FRotator::ZeroRotator,
	                                                        SpawnParameters))
	{
		EnemyClone->InitializeEnemy(GridX, GridY, EnemyDataAsset);

		EnemyClone->SetGridPosition(GridX, GridY);
		SpawnedEnemies.Add(EnemyClone);
		AttackLookupList[GetIndexFrom2D(GridX, GridY)] = 1;

		EnemyClone->AttachToActor(this, AttachmentTransformRules);
	}

	GridY++;

	// Is traversed the whole row??
	if (GridY > EnemyGridWidth - 1)
	{
		// Next row
		GridX++;

		// Is traversed the whole grid??
		if (GridX > EnemyGridHeight - 1)
		{
			EnemySpawnComplete();
			return;
		}

		GridY = 0;

		// Stop SpawnEnemiesTimerHandle
		GetWorldTimerManager().ClearTimer(SpawnEnemiesTimerHandle);

		// and start it again after a delay
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([&]
		{
			GetWorldTimerManager().SetTimer(SpawnEnemiesTimerHandle, this, &AEnemyHorde::SpawnEnemiesWithDelay,
			                                EnemySpawnDelay, true);
		});
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, EnemySpawnDelay, false);
	}
}

void AEnemyHorde::EnemySpawnComplete()
{
	UE_LOG(LogTemp, Log, TEXT("Enemy Spawn Completed"));

	TopLeftEnemy = SpawnedEnemies[GetIndexFrom2D(EnemyGridHeight - 1, 0)];
	TopRightEnemy = SpawnedEnemies.Last();

	bCanMove = true;
	EnemyDataAsset = nullptr;
	EnemyData.Empty();

	if (PlayerController == nullptr)
	{
		PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
		}
	}

	GetWorldTimerManager().ClearTimer(SpawnEnemiesTimerHandle);

	AliveEnemyCnt = SpawnedEnemies.Num();

	ChangeMovementInterval();

	// Notify GM that spawn Completed
	if (const auto GM = Cast<AUESpaceInvadersGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GM->EnemySpawnCompleted();
	}

	// Start Attack timer
	StartAttackTimer();
}
#pragma endregion


#pragma region Attack Related
void AEnemyHorde::OrderAttack()
{
	if (bCanMove) // If we can not move we also can not attack
	{
		FIntVector2 EnemyGridPos{-1, -1};
		int EnemyFrontIndex = -1;
		for (AEnemy* Enemy : SpawnedEnemies)
		{
			if (!Enemy) continue;

			EnemyGridPos = Enemy->GetGridPosition();
			if (EnemyGridPos.X == 0) // if the enemy's grid position X is 0 then attack
			{
				Enemy->Attack(ProjectileToSpawn, FMath::RandRange(MinProjectileSpeed, MaxProjectileSpeed));
			}
			else // check if the enemy's front if open to attack
			{
				EnemyFrontIndex = GetIndexFrom2D(EnemyGridPos.X - 1, EnemyGridPos.Y);
				if (AttackLookupList[EnemyFrontIndex] == 0)
				{
					Enemy->Attack(ProjectileToSpawn, FMath::RandRange(MinProjectileSpeed, MaxProjectileSpeed));
				}
			}
		}
	}

	// Restart Attack Timer
	StartAttackTimer();
}

void AEnemyHorde::StartAttackTimer()
{
	if (SpawnedEnemies.IsEmpty()) return;

	const float RndAttackRate = FMath::RandRange(MinAttackRate, MaxAttackRate);
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyHorde::OrderAttack, RndAttackRate);
}

#pragma endregion

#pragma region Helper Functions
int32 AEnemyHorde::GetIndexFrom2D(const int X, const int Y) const
{
	return X * EnemyGridWidth + Y;
}

UEnemyDataAsset* AEnemyHorde::GetEnemyData(const int Row) const
{
	for (const auto& Data : EnemyData)
	{
		if (Data->MinSpawnRowIndex == Row)
		{
			return Data;
		}
	}

	return nullptr;
}

TObjectPtr<AEnemy> AEnemyHorde::GetNewTopEnemy(const int32 GridPosX, const int32 GridPosY, const bool bIsTopLeft)
{
	TObjectPtr<AEnemy> Tmp = nullptr;
	if (bIsTopLeft) // Look for new TopLeft enemy
	{
		for (int Y = GridPosY + 1; Y < EnemyGridWidth; ++Y)
		{
			if (SpawnedEnemies[GetIndexFrom2D(GridPosX, Y)])
			{
				Tmp = SpawnedEnemies[GetIndexFrom2D(GridPosX, Y)];
				break;
			}
		}
	}
	else // Look for new TopRight enemy
	{
		for (int Y = GridPosY - 1; Y >= 0; --Y)
		{
			if (SpawnedEnemies[GetIndexFrom2D(GridPosX, Y)])
			{
				Tmp = SpawnedEnemies[GetIndexFrom2D(GridPosX, Y)];
				break;
			}
		}
	}

	return Tmp;
}

void AEnemyHorde::CheckGameOver()
{
	for (const auto& Enemy : SpawnedEnemies)
	{
		// If any of the remaining enemy inside enemy horde has reached game over location??
		if (Enemy && Enemy->GetActorLocation().X <= WorldXPosToGameOver)
		{
			// Game Over
			UE_LOG(LogTemp, Warning, TEXT("GAME OVER"));
			break;
		}
	}
}

void AEnemyHorde::ChangeMovementInterval()
{
	const float Percent = AliveEnemyCnt / (SpawnedEnemies.Num() * 1.0f);
	CurrentMovementInterval = MovementIntervalCurve->GetFloatValue(Percent);
}

#pragma endregion
