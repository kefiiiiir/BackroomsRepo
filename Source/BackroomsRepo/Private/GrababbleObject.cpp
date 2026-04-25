// Fill out your copyright notice in the Description page of Project Settings.


#include "GrababbleObject.h"

#include "AsyncTreeDifferences.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGrababbleObject::AGrababbleObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetGenerateOverlapEvents(true);
	
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComponent->SetupAttachment(Mesh);
	
	BreakFX = CreateDefaultSubobject<UParticleSystemComponent>("BreakFX");
	BreakFX->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AGrababbleObject::BeginPlay()
{
	Super::BeginPlay();
	
	if (Mesh)
	{
		Mesh->OnComponentHit.AddDynamic(this, &AGrababbleObject::OnMeshHit);
	}
}

void AGrababbleObject::OnMeshHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (!bIsBreakable)
		return;

	float ImpactStrength = NormalImpulse.Size();

	// cooldown for hit sound only (not damage)
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastHitTime < HitCooldown)
		return;

	LastHitTime = CurrentTime;

	if (ImpactStrength < breakImpulseThreshold)
		return;
	// Convert impact → damage
	float Damage = ImpactStrength * DamageMultiplier;

	// Optional clamp so weak hits still matter but don't spam 0.0001 damage
	Damage = FMath::Clamp(Damage, 1.0f, Price);

	Price -= FMath::RoundToInt(Damage);

	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),
		HitSound,
		Mesh->GetComponentLocation()
	);

	// broken condition
	if (Price <= 0)
	{
		Price = 0;

		Mesh->DestroyComponent();

		if (BreakFX)
			BreakFX->Activate();

		SetLifeSpan(2.0f);
	}
}


// Called every frame
void AGrababbleObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

