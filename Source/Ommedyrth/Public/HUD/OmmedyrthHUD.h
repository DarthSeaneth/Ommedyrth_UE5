// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OmmedyrthHUD.generated.h"

class UOmmedyrthOverlay;

UCLASS()
class OMMEDYRTH_API AOmmedyrthHUD : public AHUD
{
	GENERATED_BODY()
public:
	FORCEINLINE UOmmedyrthOverlay* GetOmmedyrthOverlay() const { return OmmedyrthOverlay; }

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = Ommedyrth)
	TSubclassOf<UOmmedyrthOverlay> OmmedyrthOverlayClass;

	UPROPERTY()
	UOmmedyrthOverlay* OmmedyrthOverlay;
};
