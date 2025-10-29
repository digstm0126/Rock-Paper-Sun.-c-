// Fill out your copyright notice in the Description page of Project Settings.


#include "ADoor.h"

#include "VectorTypes.h"
#include "Characters/EnemyCharacter.h"
#include "Characters/PlayerCharacter.h"


// Sets default values
AADoor::AADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(false);
	
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("DoorMesh"));
	DoorMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	
	DetectionCapsule = CreateDefaultSubobject<UCapsuleComponent>(FName("DetectionCapsule"));
	DetectionCapsule->AttachToComponent(DoorMesh, FAttachmentTransformRules::KeepRelativeTransform);
	
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

void AADoor::LockDoor()
{
	if (IsLocked)
	{
		UE_LOG(LogActor, Error, TEXT("Door already locked bozo"))
		return;
	}

	if (KeyForDoor == nullptr)
	{
		if (KeyToSpawn == nullptr)
		{
			UE_LOG(LogActor, Error, TEXT("Key to spawn not selected"))
			return;
		}

		if (UWorld* World = GetWorld())
		{
			const FVector SpawnLocation = GetActorLocation();
			const FRotator SpawnRotation = GetActorRotation();
			const FActorSpawnParameters SpawnParameters;

			if (AItem* SpawnedItem = Cast<AItem>(World->SpawnActor(KeyToSpawn, &SpawnLocation, &SpawnRotation, SpawnParameters)))
			{
				KeyForDoor = SpawnedItem;
			}
			else
			{
				UE_LOG(LogActor, Error, TEXT("the new key is not an item bozo"))
			}
		}
	}
	IsLocked = true;
}

bool AADoor::TryKeyToUnlock(AItem* KeyToTry)
{
	if (!IsLocked)
	{
		UE_LOG(LogActor, Error, TEXT("door isn't locked bozo"))
		return true;
	}
	if (KeyToTry == KeyForDoor)
	{
		UnlockDoor();
		return true;
	}
	return false;
}

void AADoor::UnlockDoor()
{
	if (KeyForDoor)
	{
		KeyForDoor = nullptr;
	}

	IsLocked = false;
}


// Called when the game starts or when spawned
void AADoor::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocked)
	{
		LockDoor();
	}
	else
	{
		UnlockDoor();
	}

}

void AADoor::HandelDetectionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsLocked)
	{
		return;
	}
	
	if (IsOpen)
	{
		return;
	}
	
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(OtherComp))
		{
			IsOpen = true;
			PrimaryActorTick.SetTickFunctionEnable(true);
			UE_LOG(LogActor, Log, TEXT("Player opened Door"))
		}
	}

	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor))
	{
		if (UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(OtherComp))
		{
			IsOpen = true;
			PrimaryActorTick.SetTickFunctionEnable(true);
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
		FRotator NewDoorRotation = FMath::RInterpTo(DoorMesh->GetRelativeRotation(), OpenRotation, DeltaTime, DoorSpeed);
		//set door rotation
		DoorMesh->SetWorldRotation(NewDoorRotation);
		//if open stops ticking
		if (NewDoorRotation == OpenRotation)
		{
			//stop ticking
			PrimaryActorTick.SetTickFunctionEnable(false);
		}
	}
	else
	{
		// get rotation
		FRotator NewDoorRotation = FMath::RInterpTo(DoorMesh->GetRelativeRotation(), OpenRotation, DeltaTime, DoorSpeed);
		//set door rotation
		DoorMesh->SetWorldRotation(NewDoorRotation);
		//if open stops ticking
		if (NewDoorRotation == OpenRotation)
		{
			//stop ticking
			PrimaryActorTick.SetTickFunctionEnable(false);
		}
	}
}

