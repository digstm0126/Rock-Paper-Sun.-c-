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

void ARoom::SpawnWallsTopBottom()
{
	if (UWorld* World = GetWorld())
	{
		//Spawn walls Top
		int NumOfWallsX = RoomConfiguration.RoomSizeX - 2;
		int IndexOfDoorSpot = (RoomConfiguration.RoomSizeX / 2) - 1;

		
		FVector TopWallsStartLocation = GetActorLocation() + FVector(RoomConfiguration.CellSize,0,0);
		FRotator TopWallsRotation = GetActorRotation() + FRotator(0,0,0);
		for (int WallIndexX = 0; WallIndexX < NumOfWallsX; ++WallIndexX)
		{
			FVector PieceLocation =  TopWallsStartLocation + FVector(WallIndexX * RoomConfiguration.CellSize, 0, 0);

			if (!CheckAndSpawnDoor(TopWallsRotation, PieceLocation, ECardinalDirection::North, WallIndexX))
			{
				//Spawn wall
				AActor* SpawnedPiece = World->SpawnActor<AActor>(WallRoomPiece, PieceLocation, TopWallsRotation);
				RoomPieces.Add(SpawnedPiece);
			}
		}

		//Spawn walls bottom
		FVector BottomWallsStartLocation = TopWallsStartLocation + FVector(0, (RoomConfiguration.RoomSizeY - 1) * RoomConfiguration.CellSize,0);
		FRotator BottomWallsRotation = GetActorRotation() + FRotator(0,180,0);
		for (int WallIndexX = 0; WallIndexX < NumOfWallsX; ++WallIndexX)
		{

			FVector PieceLocation =  BottomWallsStartLocation + FVector(WallIndexX * RoomConfiguration.CellSize, 0, 0);

			if (!CheckAndSpawnDoor(BottomWallsRotation, PieceLocation, ECardinalDirection::South, WallIndexX))
			{
				//Spawn wall
				AActor* SpawnedPiece = World->SpawnActor<AActor>(WallRoomPiece, PieceLocation, BottomWallsRotation);
				RoomPieces.Add(SpawnedPiece);
			}
		}
	}
}

bool ARoom::CheckAndSpawnDoor(FRotator Rotation, FVector Location, ECardinalDirection CurrentWallSide, int CurrentWallIndex)
{
	//check if Door
	bool FoundDoor = false;
	for (FDoorConfiguration DoorConfiguration : RoomConfiguration.Doors)
	{
		if (DoorConfiguration.WallSide == CurrentWallSide && (DoorConfiguration.StepsAlongWall - 1) == CurrentWallIndex)
		{
			if (UWorld* World = GetWorld())
			{
				//Spawn Door
				AActor* SpawnedPiece = World->SpawnActor<AActor>(DoorHoleRoomPiece, Location, Rotation);
				RoomPieces.Add(SpawnedPiece);
				FoundDoor = true;
			}
		}
	}
	return FoundDoor;
}

void ARoom::SpawnWallsLeftRight()
{
	if (UWorld* World = GetWorld())
	{
		int NumOfWallY = RoomConfiguration.RoomSizeY - 2;
		
		//Spawn walls Left
		FRotator LeftWallsRotation = GetActorRotation() + FRotator(0,-90,0);
		FVector LeftWallsStartLocation = GetActorLocation() + FVector(0,RoomConfiguration.CellSize,0);
		
		for (int WallIndexY = 0; WallIndexY < NumOfWallY; ++WallIndexY)
		{
			FVector PieceLocation =  LeftWallsStartLocation + FVector(WallIndexY * RoomConfiguration.CellSize, 0, 0);

			if (!CheckAndSpawnDoor(LeftWallsRotation, PieceLocation, ECardinalDirection::West, WallIndexY))
			{
				//Spawn wall
				AActor* SpawnedPiece = World->SpawnActor<AActor>(WallRoomPiece, PieceLocation, LeftWallsRotation);
				RoomPieces.Add(SpawnedPiece);
			}
		}

		//Spawn walls right
		FVector RightWallsStartLocation = LeftWallsStartLocation + FVector((RoomConfiguration.RoomSizeX - 1) * RoomConfiguration.CellSize,0,0);
		FRotator RightWallsRotation = GetActorRotation() + FRotator(0,90,0);
		
		for (int WallIndexY = 0; WallIndexY < NumOfWallY; ++WallIndexY)
		{
			FVector PieceLocation =  RightWallsStartLocation + FVector(0, WallIndexY * RoomConfiguration.CellSize, 0);

			if (!CheckAndSpawnDoor(RightWallsRotation, PieceLocation, ECardinalDirection::East, WallIndexY))
			{
				//Spawn wall
				AActor* SpawnedPiece = World->SpawnActor<AActor>(WallRoomPiece, PieceLocation, RightWallsRotation);
				RoomPieces.Add(SpawnedPiece);
			}
		}
	}
}

void ARoom::BuildRoom()
{
	DestroyRoomPieces();
	
	SpawnWallsTopBottom();

	SpawnWallsLeftRight();

	//buildDoors
	if (IndexOfDoorNorth)
	{
		
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

