// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(FName("InteractableSphere"));
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionProfileName(FName("BlockAllDynamic"));
	
	Mesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	InteractionSphere->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	EnableItem();
}

void AItem::EnableItem()
{
	Mesh->SetVisibility(true);
	InteractionSphere->SetGenerateOverlapEvents(true);
}

void AItem::DisableItem()
{
	Mesh->SetVisibility(false);
	InteractionSphere->SetGenerateOverlapEvents(false);
}


FName AItem::GetItemName()
{
	return ItemName;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

