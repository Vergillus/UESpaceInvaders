// Fill out your copyright notice in the Description page of Project Settings.


#include "Bunker/Bunker.h"

#include "Bunker/BunkerPiece.h"

// Sets default values
ABunker::ABunker() :
	Column(3),
	Row(3)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ABunker::GenerateBunker()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FAttachmentTransformRules AttachmentTransformRules{EAttachmentRule::KeepWorld,true};
	
	for (int X = 0; X < Column; ++X)
	{
		for (int Y = 0; Y < Row; ++Y)
		{
			FVector SpawnPos = GetActorLocation() + FVector{X * CellSize, Y* CellSize,0};
			if(const auto PieceClone = GetWorld()->SpawnActor<ABunkerPiece>(BunkerPieceToSpawn, SpawnPos, FRotator::ZeroRotator, SpawnParameters))
			{
				PieceClone->AttachToActor(this,AttachmentTransformRules);
			}
		}
	}
}

void ABunker::Clear()
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	for (AActor* BunkerPiece : AttachedActors)
	{
		BunkerPiece->SetLifeSpan(0.01f);
	}
}

