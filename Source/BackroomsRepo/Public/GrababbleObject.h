// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "GrababbleObject.generated.h"

UCLASS()
class BACKROOMSREPO_API AGrababbleObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrababbleObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Money")
	int Price;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Money")
	UWidgetComponent* WidgetComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Money")
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physics")
	bool bIsBreakable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physics")
	float breakImpulseThreshold;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physics")
	UParticleSystemComponent* BreakFX;
	
	UFUNCTION(BlueprintCallable)
	void OnMeshHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
		);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sounds")
	USoundWave* HitSound;
	
	float LastHitTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sounds")
	float HitCooldown = 0.2f;
};
