// Copyright Epic Games, Inc. All Rights Reserved.

#include "BackroomsRepoCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GrabComponent.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ABackroomsRepoCharacter

ABackroomsRepoCharacter::ABackroomsRepoCharacter()
{
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetMesh(), TEXT("Head"));
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	
	PhysicalAnimationComponent = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimationComponent"));

	CharacterPhysicsData.bIsLocalSimulation = false;
	CharacterPhysicsData.OrientationStrength = 1000.f;
	CharacterPhysicsData.VelocityStrength = 100.f;
	CharacterPhysicsData.PositionStrength = 1000.f;
	
	GrabComponent = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComponent"));
	
	LookLocation = CreateDefaultSubobject<USceneComponent>(TEXT("LookLocation"));
	LookLocation->SetupAttachment(GetFirstPersonCameraComponent());
	
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysHandle"));
	
	PhysicsHandle->InterpolationSpeed = 15.f;

	PhysicsHandle->LinearStiffness = 5000.f;
	PhysicsHandle->LinearDamping = 200.f;

	PhysicsHandle->AngularStiffness = 5000.f;
	PhysicsHandle->AngularDamping = 200.f;
	
	Beam = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Beam"));
	Beam->SetupAttachment(GetMesh(), FName("RightHand"));
	
	bReplicates = true;
	
	PrimaryActorTick.bCanEverTick = true;
}

void ABackroomsRepoCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	PhysicalAnimationComponent->SetSkeletalMeshComponent(GetMesh());
	PhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("Hips"), CharacterPhysicsData, false);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("Hips"), true, false);
}

//////////////////////////////////////////////////////////////////////////// Input

void ABackroomsRepoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABackroomsRepoCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABackroomsRepoCharacter::Look);
		
		// Grabbing
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, GrabComponent, &UGrabComponent::Grab);
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Completed, GrabComponent, &UGrabComponent::UnGrab);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ABackroomsRepoCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ABackroomsRepoCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABackroomsRepoCharacter::UnlockItem(FName ItemID)
{
	if (!UnlockedItems.Contains(ItemID))
	{
		UnlockedItems.Add(ItemID);

		UE_LOG(LogTemplateCharacter, Log, TEXT("Unlocked Item: %s"), *ItemID.ToString());

		if (UnlockWidgetClass)
		{
			if (!UnlockWidgetInstance)
			{
				UnlockWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), UnlockWidgetClass);
			}

			if (UnlockWidgetInstance && !UnlockWidgetInstance->IsInViewport())
			{
				UnlockWidgetInstance->AddToViewport(100);
				UnlockWidgetInstance->CallFunctionByNameWithArguments(TEXT("PlayAppearence"), *GLog, nullptr, true);
			}
		}
	}
}

void ABackroomsRepoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
