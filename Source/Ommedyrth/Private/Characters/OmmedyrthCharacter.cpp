// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/OmmedyrthCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Components/AttributeComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "HUD/OmmedyrthHUD.h"
#include "HUD/OmmedyrthOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"
#include "Items/Potion.h"

AOmmedyrthCharacter::AOmmedyrthCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Make Character Mesh move according to movement direction
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	//Set up Spring Arm Component
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	//Set up Camera Component
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	//Set up Groom Components
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AOmmedyrthCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Attributes && OmmedyrthOverlay && bCanRegenerateStamina)
	{
		Attributes->RegenerateStamina(DeltaTime);
		OmmedyrthOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

void AOmmedyrthCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		//Link Input Mapping Context to Character Player Controller
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(OmmedyrthContext, 0);
		}

		InitializeOmmedyrthOverlay(PlayerController);
	}

	Tags.Add(FName("EngageableTarget"));
}

void AOmmedyrthCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind functions to specific Input Actions 
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AOmmedyrthCharacter::Move);
		EnhancedInputComponent->BindAction(LookingAction, ETriggerEvent::Triggered, this, &AOmmedyrthCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AOmmedyrthCharacter::Jump);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Started, this, &AOmmedyrthCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AOmmedyrthCharacter::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AOmmedyrthCharacter::Dodge);
	}
}

void AOmmedyrthCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

float AOmmedyrthCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	return DamageAmount;
}

void AOmmedyrthCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->IsAlive())
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void AOmmedyrthCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AOmmedyrthCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && OmmedyrthOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		OmmedyrthOverlay->SetSouls(Attributes->GetSouls());
	}
}

void AOmmedyrthCharacter::AddGold(ATreasure* Treasure)
{
	if (Attributes && OmmedyrthOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		OmmedyrthOverlay->SetGold(Attributes->GetGold());
	}
}

void AOmmedyrthCharacter::UsePotion(APotion* Potion)
{
	if (Attributes && OmmedyrthOverlay)
	{
		Attributes->RestoreHealth(Potion->GetHealthAmount());
		OmmedyrthOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

void AOmmedyrthCharacter::Attack()
{
	Super::Attack();
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
		bCanRegenerateStamina = false;
		if (Attributes && OmmedyrthOverlay)
		{
			Attributes->UseStamina(Attributes->GetAttackCost());
			OmmedyrthOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
		}
	}
}

void AOmmedyrthCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (ActionState != EActionState::EAS_Unoccupied) { return; }
	if (GetController())
	{
		//Get Controller Rotation and use Rotation Matrix to get Controller Forward and Right Vectors
		const FRotator ControllerRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControllerRotation.Yaw, 0.f);
		const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardVector, MovementVector.Y);
		AddMovementInput(RightVector, MovementVector.X);
	}
}

void AOmmedyrthCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerPitchInput(LookAxisVector.Y);
		AddControllerYawInput(LookAxisVector.X);
	}
}

void AOmmedyrthCharacter::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		if (EquippedWeapon) EquippedWeapon->Destroy();
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
}

void AOmmedyrthCharacter::Dodge()
{
	if (!IsUnoccupied() || !HasEnoughStamina()) return;
	ActionState = EActionState::EAS_Dodge;
	PlayDodgeMontage();
	bCanRegenerateStamina = false;
	if (Attributes && OmmedyrthOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		OmmedyrthOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

void AOmmedyrthCharacter::Die_Implementation()
{
	Super::Die_Implementation();
	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}

void AOmmedyrthCharacter::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	SetHUDHealth();
}

void AOmmedyrthCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void AOmmedyrthCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
	bCanRegenerateStamina = true;
}

void AOmmedyrthCharacter::DodgeEnd()
{
	Super::DodgeEnd();
	ActionState = EActionState::EAS_Unoccupied;
	bCanRegenerateStamina = true;
}

bool AOmmedyrthCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

bool AOmmedyrthCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

bool AOmmedyrthCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon;
}

void AOmmedyrthCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AOmmedyrthCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AOmmedyrthCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void AOmmedyrthCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void AOmmedyrthCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void AOmmedyrthCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AOmmedyrthCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
	bCanRegenerateStamina = true;
}

bool AOmmedyrthCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

bool AOmmedyrthCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() >= Attributes->GetDodgeCost();
}

void AOmmedyrthCharacter::InitializeOmmedyrthOverlay(APlayerController* PlayerController)
{
	AOmmedyrthHUD* OmmedyrthHUD = Cast<AOmmedyrthHUD>(PlayerController->GetHUD());
	if (OmmedyrthHUD)
	{
		OmmedyrthOverlay = OmmedyrthHUD->GetOmmedyrthOverlay();
		if (OmmedyrthOverlay && Attributes)
		{
			OmmedyrthOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
			OmmedyrthOverlay->SetStaminaBarPercent(1.f);
			OmmedyrthOverlay->SetGold(0);
			OmmedyrthOverlay->SetSouls(0);
		}
	}
}

void AOmmedyrthCharacter::SetHUDHealth()
{
	if (Attributes && OmmedyrthOverlay)
	{
		OmmedyrthOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}
