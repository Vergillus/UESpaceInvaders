// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SIGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UESPACEINVADERS_API USIGameInstance : public UGameInstance
{
	GENERATED_BODY()

	int PlayerScore;
	const int MaxPlayerLives = 3;
	int PlayerLives = MaxPlayerLives;

public:

	FORCEINLINE void IncreaseScore(const int Score) { PlayerScore += Score;}
	FORCEINLINE int GetScore() const {return PlayerScore;}

	FORCEINLINE void DecreaseLives() { PlayerLives--;}
	FORCEINLINE int GetLives() const { return PlayerLives; }

	FORCEINLINE int GetMaxLives() const { return MaxPlayerLives; }
	
};
