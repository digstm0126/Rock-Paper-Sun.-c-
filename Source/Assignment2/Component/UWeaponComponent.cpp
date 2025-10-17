// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/UWeaponComponent.h"

#include "Characters/EnemyCharacter.h"
#include "Characters/PlayerCharacter.h"

// Sets default values for this component's properties
UUWeaponComponent::UUWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
	//if player
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(this->GetOwner()))
	{
		WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("SwordMesh"));
		Player->SetRightHeldItem(WeaponMesh);
	}
	else
	{
		UE_LOG(LogCore, Error, TEXT("weapon manager not attached to a player fix it bozo"))
	}

	BindToWeapon();
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
	WeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &UUWeaponComponent::HandelHit); 
}

void UUWeaponComponent::HandelHit(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	//check if bing used is atacking
	if (!InUses)
	{
		return;
	}
	
	//check if not user
	if (OtherActor == User)
	{
		return;
	}

	//checks if it's a Damageable Actor
	if (OtherActor->CanBeDamaged())
	{
		//check Actor is one of the hit able classes
		if (AttackableActors.Contains(OtherActor->GetClass()))
		{
			UE_LOG(LogCore, Error, TEXT("can atack"))
		}
	}
	
	//check if stagarable thing
	
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

