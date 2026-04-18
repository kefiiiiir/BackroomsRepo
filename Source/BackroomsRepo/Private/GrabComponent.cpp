// Fill out your copyright notice in the Description page of Project Settings.

#include "GrabComponent.h"

#include "BackroomsRepo/BackroomsRepoCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicsEngine/BodyInstance.h"

UGrabComponent::UGrabComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
}

void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGrabComponent::Grab(UPrimitiveComponent* Target)
{
	ABackroomsRepoCharacter* Char = Cast<ABackroomsRepoCharacter>(GetOwner());
	if (!Char) return;
	
	Char->PhysicsConstraintComponent->SetConstrainedComponents(Char->GetMesh(), FName("LeftHand"), Target, NAME_None);
	
	bGrabbing = true;
}

void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	ACharacter* Char = Cast<ACharacter>(GetOwner());
	if (!Char) return;
	
	APlayerController* PC = Cast<APlayerController>(Char->GetController());
	if (!PC) return;

	bHoldLook = PC->IsInputKeyDown(LookHoldKey);
}

