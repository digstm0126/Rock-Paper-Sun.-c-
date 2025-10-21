// Fill out your copyright notice in the Description page of Project Settings.


#include "UHealthManagerComponent.h"

// Sets default values for this component's properties
UHealthManagerComponent::UHealthManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


void UHealthManagerComponent::ReduceHealth(float AmountToReduce)
{
	if (!IsAlive)
	{
		return;
	}
	
	Health -= AmountToReduce;
	if (Health <= 0)
	{
		Health = 0;
		HandelDeath();
	}
}

void UHealthManagerComponent::AddHealth(float AmountToAdd)
{
	Health += AmountToAdd;
}

void UHealthManagerComponent::SetHealth(float NewHealth)
{
	Health = NewHealth;
}

void UHealthManagerComponent::HandelDeath()
{
	if (Health <= 0)
	{
		IsAlive = false;
		UE_LOG(LogTemp, Warning, TEXT("This thing died bozo"));
	}
	else
	{
		UE_LOG(LogCore, Error, TEXT("cant handel Death as this still has health"))
	}
}

// Called when the game starts
void UHealthManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

