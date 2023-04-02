// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInGameUI::OnPlayerDead(const int RetryAmount)
{
	if(LifeImages.IsEmpty() || RetryAmount < 0 || RetryAmount > LifeImages.Num()) return;

	LifeImages[RetryAmount]->SetVisibility(ESlateVisibility::Hidden);
}

void UInGameUI::ChangeScore(const int NewScore) const
{
	if(!ScoreText) return;
	
	ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), NewScore)));
}

void UInGameUI::OnLevelChanged(const int RetryAmount)
{
	if(LifeImages.IsEmpty() || RetryAmount < 0 || RetryAmount > LifeImages.Num()) return;

	for (int i = LifeImages.Num() - 1; i >= RetryAmount; --i)
	{
		LifeImages[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}
