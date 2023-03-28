// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyHorde.h"

#include "Enemy/Enemy.h"
#include "Enemy/EnemyDataAsset.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyHorde::AEnemyHorde() :
	EnemyGridWidth(12),
	EnemyGridHeight(5),
	GridCellSize(100),
	MovementInterval(1.0f),
	MovementCounter(0.0f),
	MovementInput(1),
	VerticalMovementDelay(0.5f),
	WorldXPosToGameOver(-200),
	bCanMove(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyHorde::BeginPlay()
{
	Super::BeginPlay();

	MovementInput = 1;

	GridCellHalfSize = GridCellSize * 0.5f;
	
	SpawnEnemies();

	GetWorldTimerManager().SetTimer(SpawnEnemiesTimerHandle, this, &AEnemyHorde::SpawnEnemiesWithDelay, 1.0f, true);
}

// Called every frame
void AEnemyHorde::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(PlayerController == nullptr)
	{
		PlayerController = UGameplayStatics::GetPlayerController(this,0);
		if (PlayerController)
		{
			PlayerController->GetViewportSize(ViewportSizeX,ViewportSizeY);

			//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Black, FString::Printf(TEXT("%i __ %i"), ViewportSizeX, ViewportSizeY));
		}
	}

	if (bCanMove)
	{
		Move(DeltaTime);
	}

}

void AEnemyHorde::Move(float DeltaTime)
{

	// Delay/Stop movement after vertical happens for a given interval
	if(VerticalMovementCounter > 0)
	{
		VerticalMovementCounter -= DeltaTime;
		return;
	}

	MovementCounter += DeltaTime;

	if (MovementCounter >= MovementInterval)
	{
		FVector LocationOffset(0, GridCellSize, 0);
		LocationOffset *= MovementInput;

		FVector BoundaryLocation = MovementInput > 0 ?  TopRightEnemy->GetActorLocation() : TopLeftEnemy->GetActorLocation();
		BoundaryLocation += LocationOffset;
		BoundaryLocation.Y += GridCellHalfSize * MovementInput;

		DrawDebugSphere(GetWorld(),BoundaryLocation, 100, 12, FColor::Red, false, 1.0f);

		FVector2D ScreenPosition;		
		PlayerController->ProjectWorldLocationToScreen(BoundaryLocation,ScreenPosition, true);		
		
		if(ScreenPosition.X > ViewportSizeX || ScreenPosition.X < -GridCellHalfSize)
		{
			MovementInput *= -1;

			AddActorWorldOffset(FVector{-GridCellSize * 0.5f,0,0});			
			VerticalMovementCounter = VerticalMovementDelay;

			CheckGameOver();

			return;
		}		
		
		AddActorWorldOffset(LocationOffset);
		MovementCounter = 0.0f;	
	}
}

void AEnemyHorde::CheckGameOver()
{
	for(const auto& Enemy : SpawnedEnemies)
	{
		if (Enemy->GetActorLocation().X <= WorldXPosToGameOver)
		{
			UE_LOG(LogTemp,Warning,TEXT("GAME OVER"));
			break;
		}
	}
}


void AEnemyHorde::EnemyDead(const int GridPosX, const int GridPosY)
{
	
}

void AEnemyHorde::SpawnEnemies()
{
	if(EnemyData.IsEmpty()) return;
	
	const auto World = GetWorld();

	if(!World) return;
	
	Grid.SetNum(EnemyGridWidth * EnemyGridHeight);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Owner = this;

	const FAttachmentTransformRules AttachmentTransformRules{EAttachmentRule::KeepWorld,true};

	UEnemyDataAsset* EnemyDataAsset = GetEnemyData(0);
	

	for (int X = 0; X < EnemyGridHeight; ++X)
	{
		for (int Y = 0; Y < EnemyGridWidth; ++Y)
		{

			if(X > EnemyDataAsset->MaxSpawnRowIndex)
			{
				EnemyDataAsset = GetEnemyData(X);
			}
			
			FVector SpawnPos = GetActorLocation() + FVector(X * GridCellSize, Y * GridCellSize, 0);
			
			if(AEnemy* EnemyClone = World->SpawnActor<AEnemy>(EnemyToSpawn,SpawnPos,FRotator::ZeroRotator,SpawnParameters))
			{
				EnemyClone->InitializeEnemy(X,Y,EnemyDataAsset);
				
				EnemyClone->SetGridPosition(X,Y);
				SpawnedEnemies.Add(EnemyClone);			
				Grid[X * Y] = 1;

				EnemyClone->AttachToActor(this,AttachmentTransformRules);
			}			
		}
	}

	TopLeftEnemy = SpawnedEnemies[GetIndexFrom2D(0, EnemyGridHeight - 1)];
	TopRightEnemy = SpawnedEnemies.Last();
}

void AEnemyHorde::SpawnEnemiesWithDelay()
{

	DenY++;

	if(DenY >= EnemyGridWidth)
	{
		DenX++;
		DenY = 0;

		GetWorldTimerManager().ClearTimer(SpawnEnemiesTimerHandle);		
		//GetWorldTimerManager().SetTimer(SpawnEnemiesTimerHandle, this, &AEnemyHorde::Deneme, 2, false);

		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		constexpr float Delay = 1.0f;
		TimerDelegate.BindLambda([&]
		{
			GetWorldTimerManager().SetTimer(SpawnEnemiesTimerHandle, this, &AEnemyHorde::SpawnEnemiesWithDelay, 1.0f, true);		
		});
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate,Delay,false);
	}

	FVector SpawnPos = GetActorLocation() + FVector(DenX * GridCellSize, DenY * GridCellSize, 0);

	UE_LOG(LogTemp,Log, TEXT("%s"),*SpawnPos.ToString());
}

void AEnemyHorde::Deneme()
{
	UE_LOG(LogTemp,Log, TEXT("UnPause"));	
	GetWorldTimerManager().SetTimer(SpawnEnemiesTimerHandle, this, &AEnemyHorde::SpawnEnemiesWithDelay, 1.0f, true);
}

int32 AEnemyHorde::GetIndexFrom2D(const int X, const int Y) const
{
	return Y * EnemyGridWidth + X;
}

UEnemyDataAsset* AEnemyHorde::GetEnemyData(const int Row) const
{
	for (const auto& Data : EnemyData)
	{
		if(Data->MinSpawnRowIndex == Row)
		{
			return Data;
		}
	}

	return nullptr;
}
