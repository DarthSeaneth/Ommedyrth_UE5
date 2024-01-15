// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Potion.h"
#include "Interfaces/PickupInterface.h"

void APotion::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->UsePotion(this);
		SpawnPickupEffect();
		SpawnPickupSound();
		Destroy();
	}
}