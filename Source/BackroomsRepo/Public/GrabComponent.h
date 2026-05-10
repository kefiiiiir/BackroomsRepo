// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrababbleObject.h"
#include "InputCoreTypes.h"
#include "Components/ActorComponent.h"
#include "GrabComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BACKROOMSREPO_API UGrabComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Grab();
	
	UFUNCTION(BlueprintCallable)
	void UnGrab();

	/** When held (default: RMB), arms are forced to extend toward the view direction. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arms|LookHold")
	FKey LookHoldKey = EKeys::RightMouseButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHoldLook;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Distance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* GrabbedObject;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGrabbing;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	USoundWave* GrabSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	USoundWave* UnGrabSound;
	
	float StrengthFactor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stiffness;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damping;

private:

	bool bWasLookHoldActive = false;

};
