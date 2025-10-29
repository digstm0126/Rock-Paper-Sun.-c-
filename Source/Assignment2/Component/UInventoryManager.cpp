// Fill out your copyright notice in the Description page of Project Settings.


#include "UInventoryManager.h"
#include "Item.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UInventoryManager::UInventoryManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	if (AActor* ActorToAttachTo = GetOwner())
	{
		UCapsuleComponent* CapsuleComponent = ActorToAttachTo->GetComponentByClass<UCapsuleComponent>();
		if (ActorToAttachTo && CapsuleComponent)
		{
			CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &UInventoryManager::HandelIntegrationOverlap);
		}
		else
		{
			UE_LOG(LogActorComponent, Error, TEXT("no collider to bind intractions to"))
		}
	}
	
}

void UInventoryManager::HandelDeath()
{
	UE_LOG(LogActor, Warning, TEXT("handled death"))
	
	//unbind check
	if (AActor* ActorToAttachTo = GetOwner())
	{
		UCapsuleComponent* CapsuleComponent = ActorToAttachTo->GetComponentByClass<UCapsuleComponent>();
		if (ActorToAttachTo && CapsuleComponent)
		{
			CapsuleComponent->OnComponentBeginOverlap.RemoveDynamic(this, &UInventoryManager::HandelIntegrationOverlap);
		}
		else
		{
			UE_LOG(LogActorComponent, Error, TEXT("no collider to remove bind interactions from"))
		}
	}
	
	//drop all the items
	for (AItem* Item : Items)
	{
		Item->SetActorLocation(GetOwner()->GetActorLocation());
		Item->EnableItem();
	}
}


// Called when the game starts
void UInventoryManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	
}

void UInventoryManager::HandelIntegrationOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	//if item pick up
	if (AItem* Item = Cast<AItem>(OtherActor))
	{
		Item->DisableItem();
		Items.Add(Item);
	}
	//if door
}


// Called every frame
void UInventoryManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

