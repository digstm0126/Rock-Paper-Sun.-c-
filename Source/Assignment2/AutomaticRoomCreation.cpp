// Fill out your copyright notice in the Description page of Project Settings.


#include "AutomaticRoomCreation.h"

#include "Room.h"

// Sets default values
AAutomaticRoomCreation::AAutomaticRoomCreation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BuildRooms();
}

// Called when the game starts or when spawned
void AAutomaticRoomCreation::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAutomaticRoomCreation::BuildRooms()
{
	if (SpawnedPlayer)
	{
		SpawnedPlayer->Destroy();
	}
	if (UWorld* World = GetWorld())
	{
		FVector RoomSpawnLocation = (GetActorLocation() + StartLocation);
		SpawnedPlayer = World->SpawnActor<AActor>(PlayerToSpawn, RoomSpawnLocation, StartRotation);
		SpawnedPlayer->Tags.Add("RF_Transient");
	}

	if (SpawnedRooms.Num() >= 1)
	{
		SpawnedRooms.Empty();
	}
	
	if (!RoomToSpawn)
	{
		UE_LOG(LogActor, Error, TEXT("no Room to Spawn set"))
		return;
	}
	for (FRoomConfiguration Room : Rooms)
	{
		if (UWorld* World = GetWorld())
		{
			FVector RoomSpawnLocation = (GetActorLocation() + StartLocation) + FVector(Room.RoomPosX, Room.RoomPosY,0);
			if (ARoom* SpawnedRoom = Cast<ARoom>(World->SpawnActor<AActor>(RoomToSpawn, RoomSpawnLocation, StartRotation)))
			{
				SpawnedRoom->Tags.Add("RF_Transient");
				SpawnedRoom->UpdateRoomConfiguration(Room);
				SpawnedRooms.Add(SpawnedRoom);
			}
		}
	}
}

// Called every frame
void AAutomaticRoomCreation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

