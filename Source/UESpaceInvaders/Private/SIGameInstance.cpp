// Fill out your copyright notice in the Description page of Project Settings.


#include "SIGameInstance.h"

#include "SISaveGame.h"
#include "Kismet/GameplayStatics.h"

void USIGameInstance::Init()
{
	Super::Init();

	LoadSaveGame();

	if (SaveGameObject)
	{
		HighScore = SaveGameObject->HighScore;
	}
}

void USIGameInstance::WriteSaveGame()
{
	SaveGameObject->HighScore = HighScore;
	UGameplayStatics::SaveGameToSlot(SaveGameObject,SaveSlotName,0);
}

void USIGameInstance::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName,0))
	{
		SaveGameObject = Cast<USISaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName,0));
	}
	else
	{
		SaveGameObject = Cast<USISaveGame>(UGameplayStatics::CreateSaveGameObject(USISaveGame::StaticClass()));
	}
}
