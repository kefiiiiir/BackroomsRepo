// Fill out your copyright notice in the Description page of Project Settings.

#include "GrabComponent.h"

#include "BackroomsRepo/BackroomsRepoCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Chaos/PBDJointConstraintData.h"
#include "GeometryCollection/Facades/CollectionPositionTargetFacade.h"

UGrabComponent::UGrabComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGrabComponent::Grab()
{
	ABackroomsRepoCharacter* Char = Cast<ABackroomsRepoCharacter>(GetOwner());
	if (!Char) return;

	if (!Char->Beam || !Char->PhysicsHandle || !Char->GetFirstPersonCameraComponent())
		return;

	FVector Start = Char->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector End = Start + Char->GetFirstPersonCameraComponent()->GetForwardVector() * Distance;

	if (Distance <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grab Distance invalid"));
		return;
	}

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Char);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	if (!bHit) return;

	UPrimitiveComponent* HitComp = Hit.GetComponent();
	AActor* HitActor = Hit.GetActor();

	if (!HitActor || !HitComp || !HitComp->IsSimulatingPhysics())
		return;

	GrabbedObject = HitActor;

	Char->PhysicsHandle->GrabComponentAtLocationWithRotation(
		HitComp,
		NAME_None,
		HitComp->GetComponentLocation(),
		HitComp->GetComponentRotation()
	);

	bIsGrabbing = true;

	if (GrabSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GrabSound, HitComp->GetComponentLocation());
	}
	
	if (GrabbedObject)
	{
		StrengthFactor = FMath::Clamp(Hit.GetComponent()->GetMass(), 0.01, 1.f);
		
		Char->PhysicsHandle->SetLinearStiffness(StrengthFactor * Stiffness);
		Char->PhysicsHandle->SetLinearDamping(StrengthFactor * Damping);
		
		AGrababbleObject* Object = Cast<AGrababbleObject>(GrabbedObject);
		
		if (Object)
			Char->UnlockItem(Object->ID);
		
		Char->SpawnMonster();
	}
}

void UGrabComponent::UnGrab()
{
	ABackroomsRepoCharacter* Char = Cast<ABackroomsRepoCharacter>(GetOwner());
	if (!Char) return;

	if (bIsGrabbing && Char->PhysicsHandle)
	{
		Char->PhysicsHandle->ReleaseComponent();
		bIsGrabbing = false;
	}

	GrabbedObject = nullptr;

	if (UnGrabSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), UnGrabSound, Char->GetActorLocation());
	}
}


void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ABackroomsRepoCharacter* Char = Cast<ABackroomsRepoCharacter>(GetOwner());
	if (!Char) return;

	UCameraComponent* Cam = Char->GetFirstPersonCameraComponent();
	if (!Cam || !Char->PhysicsHandle) return;

	APlayerController* PC = Cast<APlayerController>(Char->GetController());
	if (!PC) return;

	bHoldLook = PC->IsInputKeyDown(LookHoldKey);

	// -----------------------------
	// GRABBED STATE CHECK
	// -----------------------------
	if (bIsGrabbing && GrabbedObject)
	{
		FVector Start = Cam->GetComponentLocation();
		FVector End = GrabbedObject->GetActorLocation();

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Char);
		Params.AddIgnoredActor(GrabbedObject);

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			Hit,
			Start,
			End,
			ECC_Visibility,
			Params
		);

		if (bHit && Hit.GetActor() && Hit.GetActor()->ActorHasTag("Wall"))
		{
			UnGrab();
			return;
		}
	}

	// -----------------------------
	// TARGET UPDATE (SAFE)
	// -----------------------------
	FVector Start = Cam->GetComponentLocation();
	FVector End = Start + Cam->GetForwardVector() * Distance;

	if (Distance > 0.f)
	{
		Char->PhysicsHandle->SetTargetLocation(End);
	}
	
	Char->Beam->SetVariableVec3(
		FName("User.BeamEnd"),
		End
	);
}
