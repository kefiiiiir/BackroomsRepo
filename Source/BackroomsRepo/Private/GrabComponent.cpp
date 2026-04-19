// Fill out your copyright notice in the Description page of Project Settings.

#include "GrabComponent.h"

#include "BackroomsRepo/BackroomsRepoCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
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

void UGrabComponent::Grab()
{
	ABackroomsRepoCharacter* Char = Cast<ABackroomsRepoCharacter>(GetOwner());
	if (!Char) return;
	
	Char->Beam->Activate();
}

void UGrabComponent::UnGrab()
{
	ABackroomsRepoCharacter* Char = Cast<ABackroomsRepoCharacter>(GetOwner());
	if (!Char) return;
}

void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	ABackroomsRepoCharacter* Char = Cast<ABackroomsRepoCharacter>(GetOwner());
	if (!Char) return;
	
	APlayerController* PC = Cast<APlayerController>(Char->GetController());
	if (!PC) return;

	bHoldLook = PC->IsInputKeyDown(LookHoldKey);
}

