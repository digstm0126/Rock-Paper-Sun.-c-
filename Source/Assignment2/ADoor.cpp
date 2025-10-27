// Fill out your copyright notice in the Description page of Project Settings.


#include "ADoor.h"

#include "VectorTypes.h"
#include "Characters/PlayerCharacter.h"

// Sets default values
AADoor::AADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    if (DetectionCapsule)
    {
	    DetectionCapsule->SetCapsuleRadius(DetectionCapsuleRadius);
    	DetectionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AADoor::HandelDetectionOverlap);
    }
    else
    {
	    UE_LOG(LogActor, Error, TEXT("No detection capsule set bozo"))
    }
}

// Called when the game starts or when spawned
void AADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AADoor::HandelDetectionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsOpen)
	{
		return;
	}
	
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(OtherComp))
		{
			IsOpen = true;
			PrimaryActorTick.bCanEverTick = true;
			UE_LOG(LogActor, Log, TEXT("Player opened Door"))
		}
	}
}

// Called every frame
void AADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsOpen)
	{
		// get rotation
		FRotator NewDoorRotation = FMath::RInterpTo(ClosedRotation, OpenRotation, DeltaTime, DoorSpeed);
		//set door rotation

		//if open stops ticking
		if (NewDoorRotation == OpenRotation)
		{
			//stop ticking
			PrimaryActorTick.bCanEverTick = false;
		}
	}
	else
	{
		// get rotation
		FRotator NewDoorRotation = FMath::RInterpTo(OpenRotation, ClosedRotation, DeltaTime, DoorSpeed);
		//set door rotation
		
		//if open stops ticking
		if (NewDoorRotation == OpenRotation)
		{
			//stop ticking
			PrimaryActorTick.bCanEverTick = false;
		}
	}
}

