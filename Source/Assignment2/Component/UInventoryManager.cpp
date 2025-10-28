// Fill out your copyright notice in the Description page of Project Settings.


#include "UInventoryManager.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Item.h"
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
		UE_LOG(LogActorComponent, Warning, TEXT("hit and item yays"));
	}
	//if door
}


// Called every frame
void UInventoryManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

