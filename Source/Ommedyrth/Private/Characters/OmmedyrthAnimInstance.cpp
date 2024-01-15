//Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/OmmedyrthAnimInstance.h"
#include "Characters/OmmedyrthCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UOmmedyrthAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OmmedyrthCharacter = Cast<AOmmedyrthCharacter>(TryGetPawnOwner());
	if (OmmedyrthCharacter)
	{
		OmmedyrthCharacterMovement = OmmedyrthCharacter->GetCharacterMovement();
	}
}

void UOmmedyrthAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (OmmedyrthCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(OmmedyrthCharacterMovement->Velocity);
		isFalling = OmmedyrthCharacterMovement->IsFalling();
		CharacterState = OmmedyrthCharacter->GetCharacterState();
		ActionState = OmmedyrthCharacter->GetActionState();
		DeathPose = OmmedyrthCharacter->GetDeathPose();
	}
}