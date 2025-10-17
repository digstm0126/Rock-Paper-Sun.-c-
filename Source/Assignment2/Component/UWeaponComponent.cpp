// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/UWeaponComponent.h"

// Sets default values for this component's properties
UUWeaponComponent::UUWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UUWeaponComponent::SetUser(AActor* NewUser)
{
	if (NewUser != nullptr)
	{
		User = NewUser;
	}
	{
		UE_LOG(LogCore, Error, TEXT("weapon could not set new user as input is nullptr"))
	}
}

void UUWeaponComponent::BindToWeapon()
{
	//WeaponMesh->OnComponentBeginOverlap
}

void UUWeaponComponent::HandelHit()
{
}

void UUWeaponComponent::DealDamage(AActor* ActorToDamage)
{
}

// Called when the game starts
void UUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UUWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

