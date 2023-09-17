// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Potion.generated.h"

/**
 * 
 */
UCLASS()
class OMMEDYRTH_API APotion : public AItem
{
	GENERATED_BODY()
public:
	FORCEINLINE float GetHealthAmount() const { return HealthAmount; }
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
private:
	UPROPERTY(EditAnywhere, Category = "Potion Properties")
	float HealthAmount;
};
