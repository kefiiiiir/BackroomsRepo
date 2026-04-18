// Fill out your copyright notice in the Description page of Project Settings.


#include "GrababbleObject.h"

// Sets default values
AGrababbleObject::AGrababbleObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComponent->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AGrababbleObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrababbleObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

