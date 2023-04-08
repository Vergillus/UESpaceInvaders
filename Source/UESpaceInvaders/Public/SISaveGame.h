// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SISaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UESPACEINVADERS_API USISaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int32 HighScore;	
};
