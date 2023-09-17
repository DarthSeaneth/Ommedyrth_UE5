// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OmmedyrthHUD.h"
#include "HUD/OmmedyrthOverlay.h"

void AOmmedyrthHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && OmmedyrthOverlayClass)
		{
			OmmedyrthOverlay = CreateWidget<UOmmedyrthOverlay>(Controller, OmmedyrthOverlayClass);
			OmmedyrthOverlay->AddToViewport();
		}
	}
}
