// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SIGameInstance.generated.h"

class USISaveGame;

/**
 * 
 */
UCLASS()
class UESPACEINVADERS_API USIGameInstance : public UGameInstance
{
	GENERATED_BODY()

	int PlayerScore;
	int HighScore;

	const int MaxPlayerLives = 3;
	int PlayerLives = MaxPlayerLives;

	int CurrentLevel = 1;
	const int MaxLevel = 4;

public:

	virtual void Init() override;

	FORCEINLINE void IncreaseScore(const int Score) { PlayerScore += Score;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetScore() const {return PlayerScore;}

	FORCEINLINE void DecreaseLives() { PlayerLives--;}
	FORCEINLINE int GetLives() const { return PlayerLives; }

	FORCEINLINE int GetMaxLives() const { return MaxPlayerLives; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void IncreaseLevel() { CurrentLevel++;}
	FORCEINLINE int GetCurrentLevel() const {return CurrentLevel;}

	FORCEINLINE int GetMaxLevel() const {return MaxLevel;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetHighScore() const {return HighScore;}
	FORCEINLINE void SetHighScore(const int Score) { HighScore = FMath::Max(HighScore,Score); }
	
	UFUNCTION(BlueprintCallable)
	void WriteSaveGame();

private:	
	void LoadSaveGame();

	UPROPERTY()
	TObjectPtr<USISaveGame> SaveGameObject;
	const FString SaveSlotName = FString("SaveGame01");
	
	
};
