// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class OMMEDYRTH_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	virtual void BeginPlay() override;
	void SpawnTreasure();
	void SpawnPotion();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;

private:	

	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray< TSubclassOf<class ATreasure>> TreasureClasses;

	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray< TSubclassOf<class APotion>> PotionClasses;

	bool bBroken = false;
};
