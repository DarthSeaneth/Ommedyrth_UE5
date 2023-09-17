// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OMMEDYRTH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RegenerateStamina(float DeltaTime);
	void ReceiveDamage(float Damage);
	void UseStamina(float StaminaCost);
	void RestoreHealth(float HealthAmount);
	float GetHealthPercent();
	float GetStaminaPercent();
	bool IsAlive();
	void AddSouls(int32 NumSouls);
	void AddGold(int32 NumGold);

	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE float GetAttackCost() const { return AttackCost; }

protected:
	virtual void BeginPlay() override;

private:
	//Current Health
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;

	//Current Stamina
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Souls;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost = 30.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float AttackCost = 20.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenerationRate = 25.f;
};