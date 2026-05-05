// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "BackroomsRepoCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UGrabComponent;
class UBoxComponent;
class UStaticMeshComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ABackroomsRepoCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* GrabAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SprintAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* LanternAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Input, meta=(AllowPrivateAccess = "true"))
	UPhysicalAnimationComponent* PhysicalAnimationComponent;
	
	float NewHeight;

public:
	ABackroomsRepoCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Physics)
	FPhysicalAnimationData CharacterPhysicsData;

	/** If false, we force-disable mesh physics to prevent CharacterMovement vs physics jitter. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Physics)
	bool bAllowMeshPhysicsSimulation = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGrabComponent* GrabComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsGrabPressed = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* LookLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPhysicsHandleComponent* PhysicsHandle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* Beam;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float SprintSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float Stamina;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float MaxStamina;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float StaminaDrainRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float StaminaRegenRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float CrouchSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float CrouchHeight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	bool bIsSprinting;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	bool bIsInfiniteStamina;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float TargetFOV;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grabbing")
	AActor* HitActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grabbing")
	bool bIsGrabbing;
	
	UPROPERTY()
	float CurrentFOV;
	
	UFUNCTION(BlueprintCallable)
	void ToggleCrouch();
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FName> UnlockedItems;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> UnlockWidgetClass;

	UPROPERTY()
	UUserWidget* UnlockWidgetInstance;
	
	UFUNCTION(BlueprintCallable)
	void UnlockItem(FName ItemID);
	
	UPROPERTY()
	bool bMonsterSpawnedForThisPickup = false;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	USpotLightComponent* SpotLight;

	UFUNCTION(BlueprintCallable)
	void Lantern();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Phone;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Lives;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWantsToCrouch = false;

	float CurrentCapsuleHeight = 96.f;
	float TargetCapsuleHeight = 96.f;

	UPROPERTY(EditAnywhere)
	float CrouchInterpSpeed = 10.f;

	UPROPERTY(EditAnywhere)
	float StandingHeight = 96.f;

	UPROPERTY(EditAnywhere)
	float CrouchingHeight = 55.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* VorSpawn;
	
	UFUNCTION(BlueprintCallable)
	void SpawnMonster();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VorSpawnChance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSafeZone;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACharacter> MonsterClass;
	
	// INVENTORY
	// Simple inventory: ItemID -> Quantity
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TMap<FName, int32> Inventory;
	
	UFUNCTION(BlueprintCallable)
	void AddItem(FName ItemID, int32 Amount = 1);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(FName ItemID, int32 Amount = 1);

	UFUNCTION(BlueprintCallable)
	int32 GetItemCount(FName ItemID) const;
	
	virtual void Tick(float DeltaTime) override;
	
	
};

