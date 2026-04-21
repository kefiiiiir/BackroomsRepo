// Fill out your copyright notice in the Description page of Project Settings.


#include "GrababbleObject.h"

#include "AsyncTreeDifferences.h"

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

	if (ImpactStrength < breakImpulseThreshold)
		return;

	Mesh->DestroyComponent();

	if (BreakFX)
		BreakFX->Activate();

	SetLifeSpan(2.0f);
}


// Called every frame
void AGrababbleObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

