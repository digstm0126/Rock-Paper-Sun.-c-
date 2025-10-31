// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"

// Sets default values
ARoom::ARoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}


void ARoom::BeginDestroy()
{
	DestroyRoomPieces();
	Super::BeginDestroy();
}

void ARoom::Destroyed()
{
	DestroyRoomPieces();
	Super::Destroyed();
}

void ARoom::UpdateRoomConfiguration(FRoomConfiguration NewRoomConfiguration)
{
	RoomConfiguration = NewRoomConfiguration;
	BuildRoom();
}

void ARoom::BuildRoom()
{
	DestroyRoomPieces();
	
	if (UWorld* World = GetWorld())
	{
		AActor* SpawnedPiece = World->SpawnActor<AActor>(FloorRoomPiece, GetActorLocation(), GetActorRotation());
		RoomPieces.Add(SpawnedPiece);
	}
}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoom::DestroyRoomPieces()
{
	if (!RoomPieces.IsEmpty())
	{
		for (AActor* RoomPiece : RoomPieces)
		{
			RoomPiece->Destroy();
		}
		RoomPieces.Empty();
	}
}

// Called every frame
void ARoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

