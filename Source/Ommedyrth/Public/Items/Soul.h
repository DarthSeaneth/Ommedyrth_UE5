// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class OMMEDYRTH_API ASoul : public AItem
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE void SetSouls(int32 NumSouls) { Souls = NumSouls; }

protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	void LineTrace(const FVector& Start, const FVector& End, FHitResult& OutHit);

	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	int32 Souls;

	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	float DriftRate = -20.f;

	double DesiredZ;
};
