// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bunker.generated.h"

class ABunkerPiece;

UCLASS()
class UESPACEINVADERS_API ABunker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABunker();	

protected:
	UPROPERTY(EditAnywhere, Category= "Bunker Generation")
	int32 Column;

	UPROPERTY(EditAnywhere, Category= "Bunker Generation")
	int32 Row;

	const float CellSize = 100;

	UPROPERTY(EditAnywhere, Category= "Bunker Generation")
	TSubclassOf<ABunkerPiece> BunkerPieceToSpawn;

	/** Generates a grid of BunkerPiece actors. Called via BP Call Editor function */
	UFUNCTION(BlueprintCallable, Category= "Bunker Generation")
	void GenerateBunker();

	/** Deletes all spawned BunkerPiece actors that attached to the Bunker. Called via BP Call Editor function  */
	UFUNCTION(BlueprintCallable, Category= "Bunker Generation")
	void Clear();
};
