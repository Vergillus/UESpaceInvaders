// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.generated.h"


class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class UESPACEINVADERS_API UInGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void OnPlayerDead(const int RetryAmount);

	void ChangeScore(const int NewScore) const;

	void OnLevelChanged(const int RetryAmount);

protected:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ScoreText;

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UImage>> LifeImages;	
};
