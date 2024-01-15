// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OmmedyrthOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UOmmedyrthOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar) HealthProgressBar->SetPercent(Percent);
}

void UOmmedyrthOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressBar) StaminaProgressBar->SetPercent(Percent);
}

void UOmmedyrthOverlay::SetGold(int32 Gold)
{
	if (GoldCount)
	{
		GoldCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));
	}
}

void UOmmedyrthOverlay::SetSouls(int32 Souls)
{
	if (SoulCount)
	{
		SoulCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
	}
}
